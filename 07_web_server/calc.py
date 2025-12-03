import sys
import os

# CGI 스크립트: 표준 입력(Stdin)을 통해 POST 데이터를 수신
try:
    # 1. 표준 입력에서 전체 데이터 읽기
    content = sys.stdin.read()
    
    # 2. HTTP 헤더와 바디 분리
    # 이중 개행문자(\r\n\r\n)를 기준으로 헤더와 바디를 구분
    if "\r\n\r\n" in content:
        body = content.split("\r\n\r\n")[1]
    else:
        body = content

    # 3. 파라미터 파싱 (num1=10&num2=20 형태)
    params = {}
    for pair in body.split('&'):
        if '=' in pair:
            key, value = pair.split('=')
            params[key] = value

    # 4. 데이터 연산 수행
    n1 = int(params.get('num1', 0))
    n2 = int(params.get('num2', 0))
    result = n1 * n2

    # 5. 결과 HTML 생성 및 표준 출력(Stdout)으로 전송
    print(f"<html><body>")
    print(f"<h2>Result: {n1} X {n2} = <span style='color:red'>{result}</span></h2>")
    print(f"<br><a href='/'>Go Back</a>")
    print(f"</body></html>")

except Exception as e:
    # 예외 처리: 에러 메시지 출력
    print(f"<html><body><h2>Error: {e}</h2></body></html>")
