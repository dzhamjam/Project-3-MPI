#include <mpi.h>
#include <iostream>
#include <fstream>
#include <vector>

// Функция для вычисления последовательности
std::vector<double> compute_sequence(double r, double x0, int steps) {
    std::vector<double> sequence(steps + 1);
    sequence[0] = x0;
    for (int n = 0; n < steps; ++n) {
        sequence[n + 1] = r * sequence[n] * (1 - sequence[n]);
    }
    return sequence;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Параметры
    double r_start = 2.8;
    double r_end = 3.8;
    double r_step = 0.1;
    int steps = 100;
    double x0 = 0.5; // Начальное приближение

    // Количество значений r
    int num_r_values = static_cast<int>((r_end - r_start) / r_step) + 1;

    // Распределение значений r между процессами
    int values_per_process = num_r_values / size;
    int remainder = num_r_values % size;

    std::vector<double> local_r_values;
    for (int i = 0; i < values_per_process + (rank < remainder ? 1 : 0); ++i) {
        local_r_values.push_back(r_start + (rank * values_per_process + i + (rank < remainder ? rank : 0)) * r_step);
    }

    // Вычисление последовательностей для каждого значения r
    std::vector<std::vector<double>> local_sequences;
    for (double r : local_r_values) {
        local_sequences.push_back(compute_sequence(r, x0, steps));
    }

    std::vector<std::vector<double>> all_sequences;
    if (rank == 0) {
        all_sequences.resize(num_r_values);
    }

    std::vector<int> sendcounts(size);
    std::vector<int> displs(size);
    for (int i = 0; i < size; ++i) {
        sendcounts[i] = (values_per_process + (i < remainder ? 1 : 0)) * (steps + 1);
        displs[i] = i * values_per_process * (steps + 1) + (i < remainder ? i * (steps + 1) : 0);
    }

    std::vector<double> sendbuf;
    for (const auto& seq : local_sequences) {
        sendbuf.insert(sendbuf.end(), seq.begin(), seq.end());
    }

    std::vector<double> recvbuf(num_r_values * (steps + 1));

    MPI_Gatherv(sendbuf.data(), sendbuf.size(), MPI_DOUBLE,
                recvbuf.data(), sendcounts.data(), displs.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Вывод результатов в файл
    if (rank == 0) {
        std::ofstream output_file("sequences.txt");
        for (int i = 0; i < num_r_values; ++i) {
            output_file << "r = " << r_start + i * r_step << "\n";
            for (int j = 0; j <= steps; ++j) {
                output_file << recvbuf[i * (steps + 1) + j] << " ";
            }
            output_file << "\n";
        }
        output_file.close();
    }

    MPI_Finalize();
    return 0;
}
