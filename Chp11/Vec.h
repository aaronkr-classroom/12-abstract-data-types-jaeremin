#ifndef GUARD_H_VEC
#define GUARD_H_VEC

#include <algorithm>//max 함수
#include <cstddef>//size_t, ptrdiff_t
#include <memory>//메모리 allocator

using namespace std;//std:: max, uninitialized_fill, uninitialized_copy

template <class T> class Vec {

public:
	//인터페이스

	//타입 정의문
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef size_t size_type;
	typedef T value_type;
	typedef std::ptrdiff_t difference_type;
	typedef T& reference;
	typedef const T& const_reference;

	//생성자
	Vec() { create(); }
	explicit Vec(size_type n, const T& val = T()) {
		create(n, val);
	}
	//반복 생성자
	Vec(const Vec& v) { create(v.begin(), v.end()); }
	Vec& operator=(const Vec&); // 선언문 (이렇게 하겠습니다!)
	~Vec() { uncreate(); }//소멸자


	//벡터 크기와 인데스에 관한 새로운 함수들
	size_type size() const { return avail - data; }
	T& operator[](size_type i) { return data[i]; }
	const T& operator[](size_type i)const { return data[i]; }

	//반복자를 반환하는 새로운 함수
	iterator begin() { return data; }
	const iterator begin() const { return data; }

	iterator and () { return limit; }
	const iterator and () const { return avail; }

	//요소 추가하는 함수(메모리를 채우면, 확장합니다.)
	void push_back(const T& val)
	{
		if (avail == limit)//필요하다면 저장 공간 확보
			grow();

		unchecked_append(val); //새로운 요소 추가

	}
	//추가하는 함수
	void clear() { uncreate(); }
	bool empty() { return data == avail; }
private:
	iterator data;//vec의 첫 번째 요소를 가리키는 포인터
	iterator avail;//마지막으로 만든 요소의 다음을 가리키는 포이턴
	iterator limit; // 사용할 수 있는 마지막 요소의 다음을 가리키는 포인터

	std::allocator<T> alloc; //메모리를 할당하는 객체

	void create();
	void create(size_type, const T&);
	void create(const_iterator, const_iterator);

	void uncreate();

	void grow();
	void unchecked_append(const T&);
};

	//소별 멤버 함수
	template <class T> void Vec<T>::uncreate()
	{
		if (data)
		{
			//생성된 요소들을 역순으로 소멸
			iterator it = avail; // 마지막 요소의 가리키는 포인터
			while (it != data) // 첫번째 요소의 가리키는 포인터까지
				alloc.destroy(--it); //it감소하기

			//할당된 모든 저장 공간을 반환
			alloc.deallocate(data, limit - data);
		}
		//비어 있는 Vec을 가리키도록 포인터를 재설정
		data = avail = limit = 0;

	}

	
	//메모리 공간을 늘리는 함수
	template <class T> void Vec<T>::grow() {
		//저장 공간을 늘릴때에는 현재 사용하는 
		// 저장 공간의 2배를 할당
		size_type new_size = max(2 * (limit - data), ptrdiff_t(1));

		//새로운 저장 공간을 할당하고 기존 요소들을 새로운 저장 공간의 복사
		iterator new_data = alloc.allocate(new_size);//메모리 확보하기
		iterator new_avail = uninitialized_copy(data, avail, new_data);

		//기존의 저장 공간을 반환
		uncreate();

		//새롭게 할당한 저장 공간을 가리키려고 포인터 재설정
		data = new_data;
		avail = new_avail;
		limit = data + new_size;
	}

	//새로운 요소를 추가하기
	template <class T> void Vec<T>::unchecked_append(const T& val)
	{
		alloc.construct(avail++, val);
	}

	//생성자 멤버 함수들 (3개)
	template <class T> void Vec<T>::create()
{
		data = avail = limit = 0;
}
template <class T> void Vec<T>::create(size_type n, const T& wal)
{
	data = alloc.allocate(n);
	limit = avail = data + n;
	uninitialized_fill(data, limit, val); //allocator 비멤버 함수
}
template <class T> void Vec<T>::create(const_iterator i, const_iterator j)
{
	data = alloc.allocate(j - i);
	limit = avail = uninitialized_copy(i, j, data); //allocator 비멤버 함수
}

// = 피연산자 정의
template <class T> Vec<T>& Vec<T>::operator=(const Vec& rhs)
{
	//자가 할당 여부 확인
	if (&rhs != this)
	{
		//좌변이 지닌 배열이 점유하는 메모리를 해제
		uncreate();

		//우변이 지닌 요소를 좌변으로 복사
		create(rhs.begin(), rhs.end());
	}
	return *this;
}
#endif // !GUARD_H_VEC
