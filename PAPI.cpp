#include <iostream>
#include <papi.h>
#include <cstring>

using namespace std;
//PAPI_L3_TCM


//Add Native Event
void AddEvent()
{
	PAPI_library_init(PAPI_VER_CURRENT);

	int EventSet = PAPI_NULL;
	PAPI_create_eventset(&EventSet);

/*
unsigned int native = 0;
native = PAPI_NATIVE_MASK | 0;
PAPI_enum_event(&native, PAPI_ENUM_FIRST);
PAPI_add_event(EventSet, native);
cout << EventSet << endl;
*/
int x;
if(PAPI_event_name_to_code("BUS_TRANS_BURST:SELF", &x) == PAPI_OK)
	cout << "OK" << endl;
cout << x << endl;
if(PAPI_event_name_to_code("BUS_TRANS_BURST:BOTH_CORES", &x) == PAPI_OK)
	cout << "OK" << endl;
cout << x << endl;
}

const int CacheLen = 1024 * 1024 * 8 * 4;
int Y[CacheLen];
int X[CacheLen];

void FlushCache()
{
	for(int i = 0; i < CacheLen; i += 64)
		X[i] = i % 3;
	for(int i = 1; i < CacheLen; i += 64)
		X[i] += X[i - 1];
}

void GetCacheMissTime(int x)
{
	for(int i = 0, j = 0; i < x; i++, j = (j + 64) % CacheLen)
		Y[j] = 1;
}


int main()
{
//AddEvent();
//return 0;


//Use Event Counter	

	const int EventsCount = 1;
	int Events[EventsCount] = {PAPI_L3_TCM};
	long long values[EventsCount];

	memset(X, 0, sizeof(X));
	memset(Y, 0, sizeof(Y));

	for(int j = 1; j < 10; j++)
	{
		FlushCache();

		if(PAPI_start_counters(Events, EventsCount)!= PAPI_OK)
			cerr << "Error" << endl;
		GetCacheMissTime(j * 1024);
		if(PAPI_read_counters(values, EventsCount) != PAPI_OK)
			cerr << "Error" << endl;
		cout << j * 1024 << "\t" << values[0] << endl;

		PAPI_stop_counters(values, EventsCount);
	}
	return 0;

}