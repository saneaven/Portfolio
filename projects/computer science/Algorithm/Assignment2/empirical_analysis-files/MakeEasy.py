
import subprocess

def processIntegersFromFile(file_name):
    try:
        # 파일을 열어서 내용을 읽습니다.
        with open(file_name, 'r') as file:
            data = file.read()
        
        # 문자열 데이터를 공백으로 구분하여 정수 리스트로 변환합니다.
        numbers = list(map(int, data.split()))
        
        # 최소값, 최대값, 평균값을 계산합니다.
        min_value = min(numbers)
        max_value = max(numbers)
        avg_value = sum(numbers) / len(numbers)
        
        # print(numbers)
        print(min_value, max_value, avg_value)

        return min_value, max_value, avg_value
    
    except FileNotFoundError:
        return "파일을 찾을 수 없습니다."
    except ValueError:
        return "파일에 정수 이외의 값이 포함되어 있습니다."
    except Exception as e:
        return f"에러가 발생했습니다: {e}"
    
def writeResult():
    with open("result_summary", 'w') as output_file:
        for i in range(100, 10100, 100):
            min_val, max_val, avg_val = processIntegersFromFile(f"results/result_{i}.txt")
            print(min_val, max_val, avg_val)
            output_file.write(f"\t\t({i}, {avg_val}) +- ({avg_val - min_val}, {max_val - avg_val})\n")

    




# for i in range(100, 10100, 100):
#     with open(f"results/result_{i}.txt", "w") as f:
#         subprocess.run(["Kadane.exe", str(i) ,f"1000000"], stdout=f, stderr=subprocess.STDOUT)

writeResult()

# # 함수 사용 예시
# file_name = 'numbers.txt'  # 파일 이름
# min_val, max_val, avg_val = processIntegersFromFile(file_name)
# print(f"최소값: {min_val}, 최대값: {max_val}, 평균값: {avg_val}")
