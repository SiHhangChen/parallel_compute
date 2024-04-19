import matplotlib.pyplot as plt

def read_data_from_file(file_path):
    x_values = []
    y_values = []
    with open(file_path, 'r') as file:
        for line in file:
            parts = line.split(" ")  # 假设数据点之间使用空格分隔
            if len(parts) == 3:
                x_values.append(float(parts[0]))
                y_values.append(float(parts[2]))
    return x_values, y_values

def plot_data(x_values, y_values, title):
    plt.plot(x_values, y_values, marker='o', linestyle='-')
    plt.title(title)
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    file_path = 'BL_MPI_result.txt'  # 你的数据文件路径
    # file_path = 'BL_Openmp_result.txt'  # 你的数据文件路径
    x_values, y_values = read_data_from_file(file_path)
    
    # 按照一定数量的数据点分割数据并绘制图表
    chunk_size = 3  # 每个子数据集的大小
    num_chunks = len(x_values) // chunk_size   # 计算总共需要绘制的图表数量
    print(len(x_values),num_chunks)

    for i in range(num_chunks):
        start_index = i * chunk_size
        end_index = min((i + 1) * chunk_size, len(x_values))
        chunk_x = x_values[start_index:end_index]
        chunk_y = y_values[start_index:end_index]
        title = f'Plot {i+1}'
        plot_data(chunk_x, chunk_y, title)
