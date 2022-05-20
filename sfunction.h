#include <functional>
#include <vector>
#include <cassert>
#include <concepts>

template<typename T, std::vector<T>::size_type N, std::vector<T>::size_type M>
concept not_zero = N != 0;

template <typename T, std::vector<T>::size_type N, std::vector<T>::size_type M> requires not_zero<T, N, M>
class sfunction{
private:
	std::function<T(std::vector<T>)> evaluator;
	std::vector<T> parameters;
public:
	sfunction(std::vector<T> vec,std::function<T(std::vector<T>)> ev) {assert(("Number of paramters must be N: ", vec.size() == N));parameters = vec; evaluator = ev;}
	sfunction(std::vector<T> vec) {assert(("Number of paramters must be N: ", vec.size() == N));parameters = vec; evaluator=nullptr;}
	void set_evaluator(std::function<T(std::vector<T>)> ev) {evaluator=ev;}
	void set_parameters(std::vector<T> vec) {assert(("Number of parameters do not match: ", vec.size() == N)); parameters = vec;}
	template <typename... Args>
	T eval(T arg, Args... args);
	template <typename... Args>
	T operator()(T arg, Args... args);
	T operator[](int i) {assert(("Index out of range: ",i>=0 && i < N)); return parameters[i];}
	template <typename... Args>
	void collect_args(std::vector<T>& vargs, T arg, Args... args);
	template <typename... Args>
	void collect_args(std::vector<T>& vargs);
};

template<typename T, std::vector<T>::size_type N, std::vector<T>::size_type M>
template <typename... Args>
T sfunction<T,N,M>::eval(T arg, Args... args)
{
	assert(evaluator != nullptr);
	std::vector<T> vargs;
	vargs.push_back(arg);
	collect_args(vargs, args...);
	assert(("Number of arguments do not match function: ", vargs.size() == M));
	return evaluator(vargs);
}
template<typename T, std::vector<T>::size_type N, std::vector<T>::size_type M>
template <typename... Args>
T sfunction<T,N,M>::operator()(T arg, Args... args) 
{
	assert(evaluator != nullptr);
	std::vector<T> vargs;
	vargs.push_back(arg);
	collect_args(vargs, args...);
	assert(("Number of arguments do not match function: ", vargs.size() == M));
	return evaluator(vargs);
}
template<typename T, std::vector<T>::size_type N, std::vector<T>::size_type M>
template <typename... Args>
void sfunction<T,N,M>::collect_args(std::vector<T>& vargs, T arg, Args... args)
{
	vargs.push_back(arg);
	collect_args(vargs,args...);
}

template<typename T, std::vector<T>::size_type N, std::vector<T>::size_type M>
template <typename... Args>
void sfunction<T,N,M>::collect_args(std::vector<T>& vargs)
{
}
