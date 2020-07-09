#include "Timer.h"
#include <chrono>
#include <cstdlib>
#include <memory>
#include <iostream>
#include <random>
using namespace std;
constexpr static int kCACHE_LINE = 64;

class CacheAlignedData {
	void *buf_noalign_, *buf_;
public:
	const size_t kNUM_BYTE_;
	CacheAlignedData(const size_t kNUM_BYTE):
		buf_noalign_(malloc(kNUM_BYTE + kCACHE_LINE)),
		buf_(buf_noalign_),
		kNUM_BYTE_(kNUM_BYTE)
	{
		size_t space = kNUM_BYTE_ + kCACHE_LINE;
		if (align(kCACHE_LINE, kNUM_BYTE_, buf_, space) == nullptr) {
			abort();
		}
	}
	~CacheAlignedData()
	{
		free(buf_noalign_);
	}
	void *get() { return buf_; }
};

class LatencyBench {
	struct Node { union {
		Node *next;
		char padding[kCACHE_LINE];
	};};
	static_assert(sizeof(Node) == kCACHE_LINE, "");
	void CreateRing(Node *nodes, const size_t kRANGE_NODE);
	Node* Traverse(Node *head, const size_t kITER);
public:
	double Run(CacheAlignedData &data, const size_t kITER, const size_t kSIZE);
};
	
void LatencyBench::CreateRing(Node *nodes, const size_t kRANGE_NODE)
{
	if (kRANGE_NODE < 0 or (kRANGE_NODE&(kRANGE_NODE-1)) != 0) {
		abort();
	}
	std::default_random_engine rng;
	std::uniform_int_distribution<long> uid;
	nodes[0].next = nodes;
	for (int i = 1; i < kRANGE_NODE; ++i) {
		Node *cur = nodes + uid(rng) % (i - i%32 + 1);
		Node *nxt = cur->next;
		Node *tail = nodes + i;
		cur->next = tail;
		tail->next = nxt;
	}
}

LatencyBench::Node* LatencyBench::Traverse(Node *head, const size_t kITER)
{
	for (size_t i = 0; i < kITER; ++i) {
		head = head->next;
		head = head->next;
		head = head->next;
		head = head->next;
		head = head->next;
		head = head->next;
		head = head->next;
		head = head->next;
	}
	return head;
}

double LatencyBench::Run(CacheAlignedData &data, const size_t kITER, const size_t kRANGE_BYTE)
{
	if (kRANGE_BYTE > data.kNUM_BYTE_) {
		abort();
	}
	Node *nodes = reinterpret_cast<Node*>(data.get());
	CreateRing(nodes, kRANGE_BYTE / kCACHE_LINE);

	Timer timer;
	if (Traverse(nodes, kRANGE_BYTE / sizeof(Node)) == nullptr) { abort(); }
	timer.Start();
	if (Traverse(nodes, kITER) == nullptr) { abort(); }
	timer.Pause();
	return double(timer.get_count()) * 1000 / (kITER * 8);
}

int main()
{
	CacheAlignedData buf(size_t(1)<<30);
	LatencyBench latency_bench;
	for (size_t sz = size_t(1)<<10; sz <= size_t(1)<<26; sz *= 2) {
		cout << sz << " " << latency_bench.Run(buf, size_t(1)<<26, sz) << endl;
	}
	return 0;
}
