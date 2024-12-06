#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
#include <vector>
#include <windows.h>


class ConsoleParameter
{
public:
    static void SetColor(int text, int background)
    {
        SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
    }

    static void SetPosition(int x, int y)
    {
        COORD point;
        point.X = x;
        point.Y = y;
        SetConsoleCursorPosition(hStdOut, point);
    }

private:
    static HANDLE hStdOut;
};

HANDLE ConsoleParameter::hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);


std::mutex cout_mutex;

void draw(int id, int iterations, int num_threads)
{
    
    auto start = std::chrono::steady_clock::now();

    int colors[] = { FOREGROUND_RED, FOREGROUND_GREEN, FOREGROUND_BLUE, FOREGROUND_RED | FOREGROUND_GREEN, FOREGROUND_BLUE | FOREGROUND_GREEN };

    for (int i = 0; i < iterations; ++i)
    {
        
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));

        
        cout_mutex.lock();
        
        ConsoleParameter::SetColor(colors[id % num_threads], 0);
        ConsoleParameter::SetPosition(i, id);

        
        std::cout << '=';

       
        cout_mutex.unlock();
    }

    
    auto end = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    
    cout_mutex.lock();
    ConsoleParameter::SetPosition(iterations + 1, id);
    std::cout << " Время выполнения потока " << id << ": "
        << elapsed_time.count() << " миллисекунд\n";
    cout_mutex.unlock();
}

int main()
{
    setlocale(LC_ALL, "Russian");
    const int num_threads = 5;
    const int iterations = 50;

    std::vector<std::thread> threads;

    
    for (int i = 0; i < num_threads; ++i)
    {
        threads.emplace_back(draw, i, iterations, num_threads);
    }

    
    for (auto& t : threads)
    {
        if (t.joinable())
            t.join();
    }
    ConsoleParameter::SetPosition(0, num_threads + 1);//Эту штуку пришлось воткнуть, что бы надпись о коде возврата не наслаивалась на прогресс-бары по завершению работы программы. Другого способа что-то не придумал.
    return 0;
}
