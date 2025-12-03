#include <gtk/gtk.h>
#include <stdio.h>

// 메인 윈도우 닫기 콜백
void quit(GtkWidget *window, gpointer data)
{
    gtk_main_quit();
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *dialog;
    gint result; 

    gtk_init(&argc, &argv);

    // 1. 메인 윈도우 생성 (깨짐 방지를 위해 GUI 제목은 영어로)
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Main Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(quit), NULL);
    
    gtk_widget_show_all(window);

    // 2. 메시지 대화상자 생성 (질문 내용)
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_QUESTION,
                                    GTK_BUTTONS_YES_NO,
                                    "Have you finished all assignments?");

    // 3. 대화상자 실행 및 대기
    result = gtk_dialog_run(GTK_DIALOG(dialog));

    // 4. 응답 결과 처리 (터미널 출력은 한글로!)
    switch (result)
    {
    case GTK_RESPONSE_YES:
        g_print("사용자 응답: 예 (YES)\n -> 축하합니다! 모든 과제를 완료하셨군요.\n");
        break;
    case GTK_RESPONSE_NO:
        g_print("사용자 응답: 아니오 (NO)\n -> 조금만 더 힘내세요! 거의 다 왔습니다.\n");
        break;
    default:
        g_print("사용자 응답: 선택하지 않고 창을 닫았습니다.\n");
        break;
    }

    // 5. 대화상자 닫기
    gtk_widget_destroy(dialog);

    gtk_main();

    return 0;
}
