#include <iostream>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>
#include <chrono>
#include <cstring>
#include <list>

class ISort
{
	public:
	
	virtual void run ( int* _data , int _size ) = 0;
	
	virtual ~ISort() {};
};


class TesterArray 
{
	std::unique_ptr< ISort > itests;
	int size = 0;
	int* data = nullptr;
	public:	
	
	TesterArray( std::unique_ptr< ISort > _itests , int* _data , int _size ) : itests( std::move( _itests ) ) , data(_data ) , size( _size ) {}
	~TesterArray(){}
	
	void RunTests( void )
	{
		itests->run( data , size );			
	}
};


struct TestData
{
	int elements;
	double time;
};



class SelectionSort : public ISort
{
	public:
	
	SelectionSort(){}
	~SelectionSort() override {}
	SelectionSort( const SelectionSort& ) = delete;
	SelectionSort& operator = ( const SelectionSort& ) = delete;
	SelectionSort( const SelectionSort&& ) = delete;
	SelectionSort& operator= ( const SelectionSort&& ) = delete;
	
		
	void run ( int* data , int size ) override
	{
		selection_sort_R( data , size - 1 );
	}
	
	private:
	
	void swap ( int* src , int* dst )
	{
		int temp = *dst;
	
		*dst = *src;
	
		*src = temp;	
	}
	
	int find_max ( int* data , int size )
	{	
		int pos = 0;
	
		for( int i = 0; i <= size; i++ )
		{
			if( data[ pos ] < data[ i ] ) 
			{
				pos = i;
			}				
		}
		return pos;
	}
	
	void selection_sort ( int* data , int size )
	{
		int max = 0;
	
		for( int i = size - 1; i >= 0; i-- )
		{
			max = find_max( data , i );
	
			swap( &data[ i ] , &data[ max ] );
		}
	}
	
	void selection_sort_R ( int* data , int size )
	{	
		int max = 0;
	
		int len = size;
	
		if( len < 0 ) return;
		
		max = find_max( data , len );
	
		swap( &data[ len ] , &data[ max ] );
	
		selection_sort_R( data , size - 1 );
	}	
	
};

class HeapSort : public ISort
{
	public: 
	
	HeapSort(){}
	~HeapSort() override {}
	HeapSort( const HeapSort& ) = delete;
	HeapSort& operator = ( const HeapSort& ) = delete;
	HeapSort( const HeapSort&& ) = delete;
	HeapSort& operator = ( const HeapSort&& ) = delete;
	
	void run( int* data , int size ) override
	{
		heap_sort( data ,size );
	}
	
	private:
	
	void swap ( int* src , int* dst )
	{
		int temp = *dst;
	
		*dst = *src;
	
		*src = temp;	
	}
	
	void move_max_to_root( int* data , int root , int size )
	{
		int l = 2 * root + 1; // find_L
		int r = 2 * root + 2; // find_R
		int x = root;
	
		if( ( l < size ) && ( data[ l ] > data[ x ] ) ) x = l;
		if( ( r < size ) && ( data[ r ] > data[ x ] ) ) x = r;
		if( x == root ) return;
		swap( &data[ x ] , &data[ root ] );
	
		move_max_to_root( data , x , size - 1 );
	}

	void heap_sort( int* data , int size )
	{
	//1 построить кучу
	//1.1 найти индекс элемента с которого начнем строить кучу
	//1.2 пройтись по всем индексам( элементам ) и переместить элемент с большим значением в вершину кучи
		for( int root = ( size - 1 / 2 );/*find_P( size -1 );*/ root >= 0; root-- )
		{
			move_max_to_root( data , root , size );		
		}
	
		for( int i = size - 1; i >= 0; i-- )
		{
		// помещаем вершину кучи ( элемент с максимальным значенем ) в конец массива
			swap( &data[ 0 ] , &data[ i ] );
		// восстанавливаем кучу
			move_max_to_root( data , 0 , i );
		}
	}
};

int main ( int argc , char** argv )
{
	std::cout<<"algo7"<<std::endl;
		
	try
	{					
	
		std::vector<TestData> SSort(0);
		std::vector<TestData> HSort(0);
		
		
		#define EXP1 1
		#if EXP1
		{
			std::cout << "selection_sort elements 1000 " <<   "\n";
			
			int pool[1100] = { 0 };
			
			for( int i = 0; i < 1000; i++ )
			{
				pool[ i ] = std::rand( ) % 1000;
			}
						
			TesterArray tester( std::make_unique<SelectionSort>() , pool , 1000 );
			
			auto start = std::chrono::system_clock::now();
			
			tester.RunTests();
			
			for( int i = 0; i < 1000; i++ )
			{
				std::cout<<" "<< pool [ i ];
			}
			std::cout<<std::endl;
			
			auto end = std::chrono::system_clock::now();
       		std::chrono::duration<double> diff = end - start;
       		
       		TestData td;
       		
       		td.elements = 1000;
       		td.time = diff.count();
       		
       		SSort.push_back( td );
       		
		}
		#endif
		
		#define EXP2 1
		#if EXP2
		{
			std::cout << "heap_sort elements 1000 " <<   "\n";
			
			int pool_size = 1000;
			
			int pool2[ pool_size + 100 ] = { 0 };
			
			for( int i = 0; i < pool_size; i++ )
			{
				pool2[ i ] = std::rand( ) % pool_size;
			}
						
			TesterArray tester( std::make_unique<HeapSort>() , pool2 , pool_size );
			
			auto start = std::chrono::system_clock::now();
			
			tester.RunTests();
			
			/*
			for( int i = 0; i < pool_size; i++ )
			{
				std::cout<<" "<< pool2 [ i ];
			}
			std::cout<<std::endl;
			*/
			auto end = std::chrono::system_clock::now();
       		std::chrono::duration<double> diff = end - start;
       		
       		TestData td;
       		
       		td.elements = pool_size;
       		td.time = diff.count();
       		
       		HSort.push_back( td );
       		
		}
		#endif
		
		#define EXP3 1
		#if EXP3
		{
			int pool_size = 10000;
			
			std::cout << "selection_sort elements 10000 " <<   "\n";
			
			int pool[pool_size + 100 ] = { 0 };
			
			for( int i = 0; i < pool_size; i++ )
			{
				pool[ i ] = std::rand( ) % pool_size;
			}
						
			TesterArray tester( std::make_unique<SelectionSort>() , pool , pool_size );
			
			auto start = std::chrono::system_clock::now();
			
			tester.RunTests();
			
			auto end = std::chrono::system_clock::now();
       		std::chrono::duration<double> diff = end - start;
       		
       		TestData td;
       		
       		td.elements = pool_size;
       		td.time = diff.count();
       		
       		SSort.push_back( td );
       		
		}
		#endif
		
		#define EXP4 1
		#if EXP4
		{
			int pool_size = 100000;
			
			std::cout << "selection_sort elements 100000 " <<   "\n";
			
			int pool[pool_size + 100 ] = { 0 };
			
			for( int i = 0; i < pool_size; i++ )
			{
				pool[ i ] = std::rand( ) % pool_size;
			}
						
			TesterArray tester( std::make_unique<SelectionSort>() , pool , pool_size );
			
			auto start = std::chrono::system_clock::now();
			
			tester.RunTests();
			
			auto end = std::chrono::system_clock::now();
       		std::chrono::duration<double> diff = end - start;
       		
       		TestData td;
       		
       		td.elements = pool_size;
       		td.time = diff.count();
       		
       		SSort.push_back( td );
       		
		}
		#endif
		
		#define EXP5 0
		#if EXP5
		{
			int pool_size = 1000000;
			
			std::cout << "selection_sort elements 1000000 " <<   "\n";
			
			int pool[pool_size + 100 ] = { 0 };
			
			for( int i = 0; i < pool_size; i++ )
			{
				pool[ i ] = std::rand( ) % pool_size;
			}
						
			TesterArray tester( std::make_unique<SelectionSort>() , pool , pool_size );
			
			auto start = std::chrono::system_clock::now();
			
			tester.RunTests();
			
			auto end = std::chrono::system_clock::now();
       		std::chrono::duration<double> diff = end - start;
       		
       		TestData td;
       		
       		td.elements = pool_size;
       		td.time = diff.count();
       		
       		SSort.push_back( td );
       		
		}
		#endif
		
		#define EXP6 1
		#if EXP6
		{
			std::cout << "heap_sort elements 10000 " <<   "\n";
			
			int pool_size = 10000;
			
			int pool2[ pool_size + 100 ] = { 0 };
			
			for( int i = 0; i < pool_size; i++ )
			{
				pool2[ i ] = std::rand( ) % pool_size;
			}
						
			TesterArray tester( std::make_unique<HeapSort>() , pool2 , pool_size );
			
			auto start = std::chrono::system_clock::now();
			
			tester.RunTests();
			
			/*
			for( int i = 0; i < pool_size; i++ )
			{
				std::cout<<" "<< pool2 [ i ];
			}
			std::cout<<std::endl;
			*/
			auto end = std::chrono::system_clock::now();
       		std::chrono::duration<double> diff = end - start;
       		
       		TestData td;
       		
       		td.elements = pool_size;
       		td.time = diff.count();
       		
       		HSort.push_back( td );
       		
		}
		#endif
		
		#define EXP7 1
		#if EXP7
		{
			std::cout << "heap_sort elements 100000 " <<   "\n";
			
			int pool_size = 100000;
			
			int pool2[ pool_size + 100 ] = { 0 };
			
			for( int i = 0; i < pool_size; i++ )
			{
				pool2[ i ] = std::rand( ) % pool_size;
			}
						
			TesterArray tester( std::make_unique<HeapSort>() , pool2 , pool_size );
			
			auto start = std::chrono::system_clock::now();
			
			tester.RunTests();
			
			/*
			for( int i = 0; i < pool_size; i++ )
			{
				std::cout<<" "<< pool2 [ i ];
			}
			std::cout<<std::endl;
			*/
			auto end = std::chrono::system_clock::now();
       		std::chrono::duration<double> diff = end - start;
       		
       		TestData td;
       		
       		td.elements = pool_size;
       		td.time = diff.count();
       		
       		HSort.push_back( td );
       		
		}
		#endif
		
		#define EXP8 1
		#if EXP8
		{
			std::cout << "heap_sort elements 1000000 " <<   "\n";
			
			int pool_size = 1000000;
			
			int pool2[ pool_size + 100 ] = { 0 };
			
			for( int i = 0; i < pool_size; i++ )
			{
				pool2[ i ] = std::rand( ) % pool_size;
			}
						
			TesterArray tester( std::make_unique<HeapSort>() , pool2 , pool_size );
			
			auto start = std::chrono::system_clock::now();
			
			tester.RunTests();
			
			
			for( int i = 10000; i < 10000 + 100; i++ )
			{
				std::cout<<" "<< pool2 [ i ];
			}
			std::cout<<std::endl;
			
			auto end = std::chrono::system_clock::now();
       		std::chrono::duration<double> diff = end - start;
       		
       		TestData td;
       		
       		td.elements = pool_size;
       		td.time = diff.count();
       		
       		HSort.push_back( td );
       		
		}
		#endif
				
		
		
		for( auto a : SSort )
		{
			std::cout << "Selectsort  : elements : " << a.elements << " : time left : " << a.time << " s\n";
		}
		
		for( auto a : HSort )
		{
			std::cout << "HeapSort  : elements : " << a.elements << " : time left : " << a.time << " s\n";
		}	
	}
	catch( ... )
	{
		std::cout<<"error"<<std::endl;
	}
	
	

	return 0;
}





