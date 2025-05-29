/ 编译指令如下
// 以下为静态线程
// g++ correctness_new.cpp train.cpp guessing_static_thread.cpp md5.cpp -o main
// g++ correctness_new.cpp train.cpp guessing_static_thread.cpp md5.cpp -o main -O1
// g++ correctness_new.cpp train.cpp guessing_static_thread.cpp md5.cpp -o main -O2
// 以下为动态线程
// g++ correctness_new.cpp train.cpp guessing_dynamic_thread.cpp md5.cpp -o main
// g++ correctness_new.cpp train.cpp guessing_dynamic_thread.cpp md5.cpp -o main -O1
// g++ correctness_new.cpp train.cpp guessing_dynamic_thread.cpp md5.cpp -o main -O2
// 以下为串行
// g++ correctness_new.cpp train.cpp v md5.cpp -o main
// g++ correctness_new.cpp train.cpp guessing.cpp md5.cpp -o main -O1
// g++ correctness_new.cpp train.cpp guessing.cpp md5.cpp -o main -O2
// 以下为openmp
// g++ -fopenmp correctness_new.cpp train.cpp guessing_omp.cpp md5.cpp -o main
// g++ -fopenmp correctness_new.cpp train.cpp guessing_omp.cpp md5.cpp -o main -O1
// g++ -fopenmp correctness_new.cpp train.cpp guessing_omp.cpp md5.cpp -o main -O2

静态线程为guessing_static_thread.cpp
动态线程为guessing_dynamic_thread.cpp
串行为guessing.cpp
openmp为guessing_omp.cpp
文件夹中的guessing_static_thread_new.cpp是我尝试的pthread加速，但是试了很多次没有成功
