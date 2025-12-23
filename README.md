# 🐧 리눅스 시스템 프로그래밍 실습 보고서 (Open Source Project)

---

## 📁 보고서 목록

| 실습 | 제목 | 주요 내용 | PDF |
|:---:|:---|:---|:---:|
| **Lab1** | **리눅스 기초** | Shell Script, 빌드 도구(Make/CMake), 라이브러리 생성 | [📄&nbsp;보기](./20212979_임진호_lab1.pdf) |
| **Lab2** | **시스템 프로그래밍** | 파일 I/O, 프로세스 제어, IPC(Pipe/MQ/Sem), Rust | [📄&nbsp;보기](./20212979_임진호_Lab2.pdf) |
| **Lab3** | **네트워크 & GUI** | Thread 동기화, TCP/UDP 소켓, GTK+ GUI 구현 | [📄&nbsp;보기](./20212979_임진호_Lab3.pdf) |
> 🔍 **참고:** 각 보고서에는 소스 코드 구현, 실행 결과 터미널 캡처, 디버깅(GDB) 과정 및 분석 내용이 상세히 포함되어 있습니다.

---

## 🛠️ 개발 환경 및 기술 스택

- **OS**: Windows Subsystem for Linux 2 (Ubuntu 22.04 LTS)
- **Language**: C, Rust, Bash Shell Script
- **Build Tools**: GCC, Make, CMake
- **Debugging**: GDB, Valgrind, gprof
- **Libraries**: POSIX Standard (pthread, socket), GTK+, System V IPC

---

## 💻 주요 실습 구현 내용

### 1. 시스템 및 개발 도구 (Lab 1)
- **Bash Scripting**: 구구단 출력, 피보나치 수열, 파일 관리 자동화 스크립트 작성
- **Library Linkage**: 정적(.a), 공유(.so), 동적 로딩(dlopen) 라이브러리 제작 및 성능 비교
- **Build Automation**: Makefile 및 CMakeLists.txt 작성을 통한 빌드 자동화

### 2. 프로세스 및 IPC (Lab 2)
- **System Call**: `ls -R` 명령어 직접 구현 (재귀적 디렉토리 탐색)
- **Process Control**: `fork()`, `exec()`, `wait()`를 활용한 쉘(Shell) 동작 원리 구현
- **IPC Chat**: 파이프(Pipe), FIFO, 메시지 큐(Message Queue)를 활용한 양방향 채팅 프로그램
- **Memory Mapping**: `mmap()`을 이용한 고속 파일 복사 유틸리티 구현
- **Rust Programming**: Rust의 소유권 개념 실습 (행렬 덧셈, 전화번호부)

### 3. 스레드 및 네트워크 (Lab 3)
- **Multi-Threading**: 생산자-소비자 문제(Producer-Consumer Problem) 해결 (Mutex, Cond Var 활용)
- **Socket Programming**: 
    - TCP/UDP 에코 서버 및 클라이언트
    - `select()`를 이용한 I/O 멀티플렉싱 채팅 서버
    - GET/POST 및 CGI를 지원하는 간단한 웹 서버 구현
- **GUI**: GTK+를 활용한 계산기 프로그램 제작

---

## 🎯 학습 성과

- 리눅스 커널의 **시스템 호출(System Call)** 인터페이스에 대한 깊은 이해
- **멀티 프로세스 vs 멀티 스레드** 모델의 차이점과 동기화 문제 해결 능력 배양
- **IPC(프로세스 간 통신)** 기법의 종류별 특징과 적재적소 활용 능력 습득
- **네트워크 프로토콜(TCP/IP)**의 이해 및 소켓을 활용한 서버-클라이언트 구조 설계
