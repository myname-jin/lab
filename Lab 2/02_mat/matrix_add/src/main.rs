use std::io::{self, Write};

fn read_usize(prompt: &str) -> usize {
    loop {
        print!("{prompt}");
        // print! 출력이 바로 보이도록 flush
        io::stdout().flush().unwrap();

        let mut line = String::new();
        io::stdin().read_line(&mut line).expect("입력 오류");

        match line.trim().parse::<usize>() {
            Ok(v) if v > 0 => return v,
            _ => {
                println!("양의 정수를 입력하세요.");
            }
        }
    }
}

fn read_matrix(rows: usize, cols: usize, name: &str) -> Vec<Vec<i32>> {
    println!("{name} 행렬의 원소를 입력하세요 (각 행마다 {cols}개씩):");
    // rows 개의 행을 담기 위해 미리 capacity를 지정
    let mut mat: Vec<Vec<i32>> = Vec::with_capacity(rows);
    for r in 0..rows {
        loop {
            print!("행 {}/{}: ", r + 1, rows);
            io::stdout().flush().unwrap();

            let mut line = String::new();
            io::stdin().read_line(&mut line).expect("입력 오류");

            let nums: Vec<i32> = line
                .split_whitespace()
                .filter_map(|s| s.parse().ok())
                .collect();
            if nums.len() == cols {
                mat.push(nums);
                break;
            } else {
                println!("정확히 {cols}개의 정수를 입력해야 합니다. 다시 입력하세요.");
            }
        }
    }
    mat
}

fn add_matrices(a: &Vec<Vec<i32>>, b: &Vec<Vec<i32>>) -> Vec<Vec<i32>> {
    let rows = a.len();
    let cols = if rows > 0 { a[0].len() } else { 0 };

    let mut c: Vec<Vec<i32>> = Vec::with_capacity(rows);

    for r in 0..rows {
        let mut row: Vec<i32> = Vec::with_capacity(cols);
        for c_idx in 0..cols {
            row.push(a[r][c_idx] + b[r][c_idx]);
        }
        c.push(row);
    }

    c
}

fn print_matrix(mat: &Vec<Vec<i32>>, name: &str) {
    println!("{name} 행렬:");
    for row in mat {
        for val in row {
            print!("{:4}", val);
        }
        println!();
    }
}

fn main() {
    println!("=== 두 행렬의 합 (Rust Vec<T> 이용) ===");

    let rows = read_usize("행의 개수를 입력하세요: ");
    let cols = read_usize("열의 개수를 입력하세요: ");

    let a = read_matrix(rows, cols, "첫 번째");
    let b = read_matrix(rows, cols, "두 번째");

    let c = add_matrices(&a, &b);

    println!();
    print_matrix(&a, "A");
    print_matrix(&b, "B");
    print_matrix(&c, "A + B");
}

