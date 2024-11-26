#include <gtest/gtest.h>  
#include <vector>  

// Функция, которую мы хотим протестировать  
std::vector<double> compute_sequence(double r, double x0, int steps) {
    std::vector<double> sequence(steps + 1);
    sequence[0] = x0;
    for (int n = 0; n < steps; ++n) {
        sequence[n + 1] = r * sequence[n] * (1 - sequence[n]);
    }
    return sequence;
}

// Тест 1: Начальное значение 0 
TEST(ComputeSequenceTest, Ranges) {
    auto result = compute_sequence(3.0, 0.1, 5);
    std::vector<double> expected = { 0.1, 0.27, 0.5913, 0.72499, 0.598138, 0.72111 };
    for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_NEAR(result[i], expected[i], 1e-4); // Сравнение с учетом погрешности  
    }
}

// Тест 2: Начальное значение 0 
TEST(ComputeSequenceTest, ZeroInitialValue) {
    auto result = compute_sequence(2.0, 0.0, 10);
    EXPECT_EQ(result[0], 0.0); // Первое значение должно быть 0  
    for (size_t i = 1; i < result.size(); ++i) {
        EXPECT_EQ(result[i], 0.0); // Все последующие значения должны быть 0  
    }
}

// Тест 3
TEST(ComputeSequenceTest, MaxSteps) {
    auto result = compute_sequence(0.5, 0.1, 10);
    EXPECT_NEAR(result[0], 0.1, 1e-2); // Проверяем первое значение  
    // Проверяем, что последовательность не уходит в бесконечность  
    EXPECT_LE(result.back(), 0.5); // Последние значения должны быть в диапазоне (0,0.5)  
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
