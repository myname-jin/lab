#include <gtk/gtk.h>
#include <stdio.h>

// 버튼을 클릭했을 때 실행될 콜백 함수
static void print_hello(GtkWidget *widget, gpointer data)
{
    g_print("Hello World Clicked!\n");
}

// 윈도우 닫기 버튼(x)을 눌렀을 때 프로그램을 종료하는 콜백 함수 
static void destroy(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *button;

    // 1. GTK 환경 초기화 
    gtk_init(&argc, &argv);

    // 2. 윈도우 생성 
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "hello_gui");
    // 윈도우 크기 및 테두리 설정 
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    // 윈도우 종료 시그널 연결 (창 닫으면 프로그램 종료)
    g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);
    // 3. 버튼 생성 
    button = gtk_button_new_with_label("Hello World");
    // 4. 버튼 시그널 연결 
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
    // 5. 윈도우에 버튼 추가
    gtk_container_add(GTK_CONTAINER(window), button);
    // 6. 위젯 화면 표시
    gtk_widget_show_all(window);
    // 7. 메인 루프 실행 
    gtk_main();

    return 0;
}
