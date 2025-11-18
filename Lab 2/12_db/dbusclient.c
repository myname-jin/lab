#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <systemd/sd-bus.h>

#define BUS_NAME "org.example.NotificationService"
#define OBJECT_PATH "/org/example/NotificationService"
#define INTERFACE_NAME "org.example.NotificationService"

// 1. Tick 시그널이 도착했을 때 호출될 핸들러 함수
static int on_tick_signal(sd_bus_message *m, void *userdata, sd_bus_error *ret_error)
{
    uint32_t count = 0;
    int ret;

    ret = sd_bus_message_read(m, "u", &count);
    if (ret < 0) return ret;

    printf("[Client] 알림 수신: Tick (Count=%u)\n", count);
    return 0;
}

int main(void)
{
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message *m = NULL;
    sd_bus *bus = NULL;
    sd_bus_slot *sigslot = NULL;
    const char *reply_string;
    uint32_t property_count = 0;
    int ret;

    ret = sd_bus_default_user(&bus);
    if (ret < 0) {
        fprintf(stderr, "Failed to connect to dbus-daemon: %s\n", strerror(-ret));
        goto finish;
    }

    printf("--- DBUS 클라이언트 시작 ---\n");

    // 2. Hello 메서드 호출
    ret = sd_bus_call_method(bus, BUS_NAME, OBJECT_PATH, INTERFACE_NAME, "Hello",
                             &error, &m, "s", "Client A");
    if (ret < 0) {
        fprintf(stderr, "Failed to issue Hello method call: %s\n", error.message);
        goto finish;
    }

    // 3. 응답 메시지를 읽어서 return 값 획득
    ret = sd_bus_message_read(m, "s", &reply_string);
    if (ret < 0) goto finish;
    printf("[Client] 서버 응답: %s\n", reply_string);

    // 4. Count 속성 획득
    ret = sd_bus_get_property_trivial(bus, BUS_NAME, OBJECT_PATH, INTERFACE_NAME,
                                      "Count", &error, 'u', &property_count);
    if (ret < 0) goto finish;
    printf("[Client] 서버 속성 확인: Count = %u\n", property_count);


    // 5. Tick Signal 수신을 위한 매치 등록
    ret = sd_bus_match_signal(bus, &sigslot, BUS_NAME, OBJECT_PATH, INTERFACE_NAME, "Tick", on_tick_signal, NULL);
    if (ret < 0) {
        fprintf(stderr, "Failed to add match: %s\n", strerror(-ret));
        goto finish;
    }

    printf("Tick 알림을 수신 대기합니다 (10초)...\n");

    // 6. 10초 동안 알림 메시지 대기 루프
    for (int i = 0; i < 10; i++) { // 1초씩 10번 대기
        ret = sd_bus_process(bus, NULL);
        if (ret < 0) break;
        if (ret == 0) {
            // 메시지가 없으면 1초 대기
            sd_bus_wait(bus, (uint64_t)(1000 * 1000)); 
        }
    }

finish:
    sd_bus_error_free(&error);
    sd_bus_message_unref(m);
    sd_bus_unref(bus);
    return ret < 0 ? 1 : 0;
}
