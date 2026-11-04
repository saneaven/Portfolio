import time
import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path
from PIL import Image
import pandas as pd

import sys
sys.path.append(str(Path(__file__).parent.parent))
from source.operations.fourier_transform.fast import fastFourierTransform
from source.operations.fourier_transform.direct import directFourierTransform
from source.operations.fourier_transform.direct import separableFourierTransform

CURRENT_DIR = Path(__file__).parent

IMAGE_FOLDER = CURRENT_DIR / "images"


def run_analysis():
    # 1. 경로 설정
    base_dir = IMAGE_FOLDER
    save_dir = IMAGE_FOLDER / "fft_results"



    images = [p for p in base_dir.iterdir()][:5]

            # 사이즈별 시간 저장소: {64: {'Direct':[], ...}, 32: ...}
    sizes = [32, 16, 8, 4, 2, 1]
    result_data = {s: {'Direct': [], 'Sep': [], 'Fast': []} for s in sizes}
    csv_records = []

    for img_path in images:
        print(f"Processing: {img_path.name}")
        
        # 32x32로 초기화
        img = Image.open(img_path).convert("RGB").resize((sizes[0], sizes[0]), Image.Resampling.BILINEAR)
        
        for N in sizes: # Iterate over sizes
            # Resize image
            curr = img.resize((N, N), Image.Resampling.BILINEAR)
            
            # Direct
            s = time.perf_counter()
            for _ in range(10): direct_image, _ = directFourierTransform(curr, mode="color", bins=10)
            t_direct = (time.perf_counter() - s)/10
            result_data[N]['Direct'].append(t_direct)

            csv_records.append({
                'Filename': img_path.name,
                'Size': N,
                'Method': 'Direct',
                'Time': t_direct
            })

            # Separable
            s = time.perf_counter()
            for _ in range(10): separable_image, _ = separableFourierTransform(curr, mode="color", bins=10)
            t_separable = (time.perf_counter() - s)/10
            result_data[N]['Sep'].append(t_separable)

            csv_records.append({
                'Filename': img_path.name,
                'Size': N,
                'Method': 'Separable',
                'Time': t_separable
            })

            # Fast
            s = time.perf_counter()
            for _ in range(10): fast_image, _ = fastFourierTransform(curr, mode="color", bins=10)
            t_fast = (time.perf_counter() - s)/10
            result_data[N]['Fast'].append(t_fast)

            csv_records.append({
                'Filename': img_path.name,
                'Size': N,
                'Method': 'Fast',
                'Time': t_fast
            })

            print(f" Size {N}x{N}: Direct={t_direct:.6f}s, Sep={t_separable:.6f}s, Fast={t_fast:.6f}s")
            if N == 32:
                direct_image.save(save_dir / f"{img_path.stem}_Direct.png")
                separable_image.save(save_dir / f"{img_path.stem}_Sep.png")
                fast_image.save(save_dir / f"{img_path.stem}_Fast.png")

    # Save CSV
    df = pd.DataFrame(csv_records)
    df.to_csv(save_dir / "fft_performance.csv", index=False)
    # Draw graph
    plt.figure(figsize=(10, 6))
    
    # 평균 시간 계산 및 플롯
    avg_d = [np.mean(result_data[n]['Direct']) for n in sizes]
    avg_s = [np.mean(result_data[n]['Sep']) for n in sizes]
    avg_f = [np.mean(result_data[n]['Fast']) for n in sizes]

    plt.plot(sizes, avg_d, 'o--', label='Direct')
    plt.plot(sizes, avg_s, 's-.', label='Separable')
    plt.plot(sizes, avg_f, '^', linestyle='-', label='Fast')

    plt.xticks(sizes, sizes)
    plt.xlabel('Size (NxN)')
    plt.ylabel('Time (sec)')
    plt.legend()
    plt.grid(True)
    
    plt.savefig(Path.cwd() / 'result_graph.png')
    plt.show()

if __name__ == "__main__":
    run_analysis()