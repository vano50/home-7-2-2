#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>


std::mutex cout_mutex;

void thread_function(int id, int length)
{

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i <= length; ++i)
    {
        cout_mutex.lock();

        std::cout << "Поток " << id << ":  ";

        for (int j = 0; j < i; ++j)
            std::cout << "=";
        for (int j = i; j < length; ++j)
            std::cout << " ";
        std::cout << "]" << std::endl;

        cout_mutex.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    auto end = std::chrono::steady_clock::now();

    cout_mutex.lock();
    std::cout << "Поток " << id << " время "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " мс\n";
    cout_mutex.unlock();
}

int main()
{
    setlocale(LC_ALL, "Russian");
    const int num_threads = 5;
    const int length = 50;

    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i)
    {
        threads.emplace_back(thread_function, i + 1, length);

    }

    for (auto& t : threads)
    {
        if (t.joinable())
            t.join();
    }

    return 0;
}
