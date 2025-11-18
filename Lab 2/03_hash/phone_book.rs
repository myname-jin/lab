use std::collections::HashMap;
use std::io;

fn main() {
    let mut phone_book = HashMap::new();

    phone_book.insert("홍길동".to_string(), "010-1111-1111".to_string());
    phone_book.insert("김철수".to_string(), "010-2222-2222".to_string());
    phone_book.insert("이영희".to_string(), "010-3333-3333".to_string());

    println!("--- Rust 전화번호부 ---");

    loop {
        println!("\n검색할 이름을 입력하세요 (종료하려면 '종료'):");

        let mut input = String::new();

        // 5. 키보드로 입력을 받습니다. (panic 대신 match로 수정)
        // PDF 33페이지의 Result<T, E> 처리와 같은 원리입니다.
        match io::stdin().read_line(&mut input) {
            Ok(_) => {
                // 입력 성공. 다음으로 넘어갑니다.
            }
            Err(error) => {
                // 입력 실패!
                println!("! 입력을 읽는 중 에러가 발생했습니다: {}", error);
                // 프로그램이 죽는 대신, 루프의 처음으로 돌아갑니다.
                continue;
            }
        }

        // 6. 입력받은 문자열의 앞뒤 공백과 줄바꿈(\n) 문자를 제거합니다.
        let name = input.trim();

        // 7. "종료"라고 입력하면 loop를 빠져나갑니다.
        if name == "종료" {
            println!("프로그램을 종료합니다.");
            break;
        }

        // 8. get 함수로 HashMap에서 이름을 검색합니다.
        match phone_book.get(name) {
            Some(phone_number) => {
                println!(" -> {} 님의 전화번호는 {} 입니다.", name, phone_number);
            }
            None => {
                println!(" -> '{}' 님은 등록되어 있지 않습니다.", name);
            }
        }
    }
}
