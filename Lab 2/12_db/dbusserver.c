#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <systemd/sd-bus.h>

// --- 오류 해결을 위해 함수 프로토타입을 미리 선언 ---
static int hello_method(sd_bus_message *m, void *userdata, sd_bus_error *error);
static int get_count_property(sd_bus *bus, const char *path, const char *interface,
                              const char *property, sd_bus_message *reply, void *userdata, sd_bus_error *error);
// --------------------------------------------------------

#define BUS_NAME "org.example.NotificationService"
#define OBJECT_PATH "/org/example/NotificationService"
#define INTERFACE_NAME "org.example.NotificationService"

// 서버의 상태를 유지할 변수
static uint32_t count = 0;
static sd_bus *bus = NULL;

// 서버의 메서드 및 속성을 정의하는 테이블
static const sd_bus_vtable service_vtable[] = {
    SD_BUS_VTABLE_START(0),
    SD_BUS_METHOD("Hello", "s", "s", hello_method, SD_BUS_VTABLE_UNPRIVILEGED),
    SD_BUS_PROPERTY("Count", "u", get_count_property, 0, SD_BUS_VTABLE_PROPERTY_CONST),
    SD_BUS_VTABLE_END
};

// --- 함수 정의 ---
static int hello_method(sd_bus_message *m, void *userdata, sd_bus_error *error)
{
    const char *client_name;
    char reply[100];
    int ret;

    ret = sd_bus_message_read(m, "s", &client_name);
    if (ret < 0) return ret;

    sprintf(reply, "Hello %s! Server has been called %d times.", client_name, ++count);

    return sd_bus_reply_method_return(m, "s", reply);
}

static int get_count_property(sd_bus *bus, const char *path, const char *interface,
                              const char *property, sd_bus_message *reply, void *userdata, sd_bus_error *error)
{
    // 현재 count 값을 클라이언트에게 'u'(uint32_t) 형태로 반환
    uint32_t *pcount = (uint32_t *)userdata;
    return sd_bus_message_append(reply, "u", *pcount);
}


int main(void)
{
    sd_bus_slot *slot = NULL;
    int ret;
    uint32_t tick_count = 0;
    
    ret = sd_bus_default_user(&bus);
    if (ret < 0) {
        fprintf(stderr, "Failed to connect to dbus-daemon: %s\n", strerror(-ret));
        goto finish;
    }

    ret = sd_bus_add_object_vtable(bus, &slot, OBJECT_PATH, INTERFACE_NAME, service_vtable, &count);
    if (ret < 0) {
        fprintf(stderr, "Failed to add object interface: %s\n", strerror(-ret));
        goto finish;
    }

    ret = sd_bus_request_name(bus, BUS_NAME, 0);
    if (ret < 0) {
        fprintf(stderr, "Failed to acquire name: %s\n", strerror(-ret));
        goto finish;
    }

    printf("--- DBUS 서버 시작 (%s) ---\n", BUS_NAME);
    printf("알림 (Tick Signal)을 1초마다 전송합니다.\n");

    // --- 메인 루프: 알림 송신 및 버스 메시지 처리 ---
    while (1) {
        ret = sd_bus_process(bus, NULL);
        if (ret < 0) break;
        if (ret == 0) {
            sd_bus_wait(bus, (uint64_t)(1000 * 1000)); // 1초 대기
        }

        tick_count++;
        ret = sd_bus_emit_signal(bus, OBJECT_PATH, INTERFACE_NAME, "Tick", "u", tick_count);
        if (ret < 0) {
            fprintf(stderr, "Failed to emit signal: %s\n", strerror(-ret));
            break;
        }

        fprintf(stderr, "[Server] Tick Sent (Count=%u)\n", tick_count);
    }

finish:
    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);
    return ret < 0 ? 1 : 0;
}
