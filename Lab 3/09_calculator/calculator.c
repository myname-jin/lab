#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

GtkWidget *entry;      // 결과 표시창
int new_input = 1;     // 1이면 다음 입력 시 화면을 새로 씀 (초기화 or 계산 완료 직후)

// [숫자 버튼 핸들러] 화면에 숫자 이어 붙이기
static void on_num_clicked(GtkWidget *widget, gpointer data) {
    const char *text = gtk_button_get_label(GTK_BUTTON(widget));
    const char *current = gtk_entry_get_text(GTK_ENTRY(entry));
    char buffer[256];

    if (new_input) {
        // 새로 입력해야 하는 상태면 기존 내용을 지우고 씀
        strcpy(buffer, text);
        new_input = 0;
    } else {
        // 아니면 뒤에 이어 붙임 (예: "3" -> "3*3")
        // "0"만 있는 상태면 교체
        if (strcmp(current, "0") == 0) strcpy(buffer, text);
        else sprintf(buffer, "%s%s", current, text);
    }
    gtk_entry_set_text(GTK_ENTRY(entry), buffer);
}

// [연산자 버튼 핸들러] 화면에 연산자 이어 붙이기
static void on_op_clicked(GtkWidget *widget, gpointer data) {
    const char *op = gtk_button_get_label(GTK_BUTTON(widget));
    const char *current = gtk_entry_get_text(GTK_ENTRY(entry));
    char buffer[256];

    // 현재 수식 뒤에 연산자 붙이기 (예: "3" -> "3*")
    sprintf(buffer, "%s%s", current, op);
    gtk_entry_set_text(GTK_ENTRY(entry), buffer);
    
    new_input = 0; // 연산자 뒤에는 숫자가 이어져야 하므로 덮어쓰기 모드 해제
}

// [계산(=) 버튼 핸들러] 화면의 수식을 파싱해서 계산
static void on_calc_clicked(GtkWidget *widget, gpointer data) {
    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
    char buffer[256];
    strcpy(buffer, text);

    // 연산자 찾기 (+, -, *, /)
    // buffer + 1부터 찾는 이유는 음수(-3)로 시작하는 경우를 대비하기 위함
    char *p = strpbrk(buffer + 1, "+-*/");
    
    if (p == NULL) return; // 연산자가 없으면 종료

    char op = *p;  // 연산자 추출
    *p = '\0';     // 문자열 분리 (예: "3*3" -> "3"과 "3")

    double n1 = atof(buffer);   // 앞부분 숫자
    double n2 = atof(p + 1);    // 뒷부분 숫자 (연산자 다음 위치)
    double result = 0;

    switch (op) {
        case '+': result = n1 + n2; break;
        case '-': result = n1 - n2; break;
        case '*': result = n1 * n2; break;
        case '/': 
            if(n2 != 0) result = n1 / n2; 
            else result = 0; 
            break;
    }

    char res_str[256];
    sprintf(res_str, "%.2f", result); // 소수점 2자리 표시
    gtk_entry_set_text(GTK_ENTRY(entry), res_str);
    
    new_input = 1; // 계산 완료 후에는 새로운 입력 준비
}

// [초기화(C) 버튼 핸들러]
static void on_clear_clicked(GtkWidget *widget, gpointer data) {
    gtk_entry_set_text(GTK_ENTRY(entry), "0");
    new_input = 1;
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;
    
    // 버튼 라벨 배치도 (1-2-3 상단 배치 유지)
    char *buttons[] = {
        "1", "2", "3", "/",
        "4", "5", "6", "*",
        "7", "8", "9", "-",
        "C", "0", "=", "+"
    };

    gtk_init(&argc, &argv);

    // 1. 메인 윈도우 설정
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 300);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 2. 그리드(Grid) 레이아웃 생성
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // 3. 결과 표시창(Entry) 생성
    entry = gtk_entry_new();
    gtk_entry_set_alignment(GTK_ENTRY(entry), 1); // 오른쪽 정렬
    gtk_entry_set_text(GTK_ENTRY(entry), "0");
    gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE); // 키보드 입력 방지
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 4, 1); 

    // 4. 버튼 생성 및 배치 루프
    for (int i = 0; i < 16; i++) {
        button = gtk_button_new_with_label(buttons[i]);
        int x = i % 4;       
        int y = (i / 4) + 1; 

        // 버튼 종류에 따라 다른 핸들러 연결
        if (strcmp(buttons[i], "=") == 0) {
            g_signal_connect(button, "clicked", G_CALLBACK(on_calc_clicked), NULL);
        } else if (strcmp(buttons[i], "C") == 0) {
            g_signal_connect(button, "clicked", G_CALLBACK(on_clear_clicked), NULL);
        } else if (strchr("+-*/", buttons[i][0])) { 
            g_signal_connect(button, "clicked", G_CALLBACK(on_op_clicked), NULL);
        } else { 
            g_signal_connect(button, "clicked", G_CALLBACK(on_num_clicked), NULL);
        }

        gtk_widget_set_hexpand(button, TRUE);
        gtk_widget_set_vexpand(button, TRUE);
        gtk_grid_attach(GTK_GRID(grid), button, x, y, 1, 1);
    }

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
