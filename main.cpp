#include <chrono>
#include <memory>
#include <iostream>
#include <random>

class Timer {
	typedef std::chrono::time_point<std::chrono::high_resolution_clock> Clock;
	long long count;
	bool running;
	Clock prev_start_;
	Clock Now() {
		return std::chrono::high_resolution_clock::now();
	}
public:
	void Start() {
		running = true;
		prev_start_ = Now();
	}
	void Pause() {
		if (running) {
			running = false;
			auto diff = Now() - prev_start_;
			count += std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
		}
	}
	void Reset() {
		running = false;
		count = 0;
	}
	long long get_count() {
		return count;
	}
	Timer() {Reset();}
};

class LatencyBench {
	const long long kNUM_BYTE, kNUM_NODE;
public:
	struct Node { Node *next; } __attribute__ ((aligned (64)));
	LatencyBench(const long long kNUM_BYTE):
		kNUM_BYTE(kNUM_BYTE),
		kNUM_NODE(kNUM_BYTE / sizeof(Node))
	{
		nodes_.reset(new Node[kNUM_NODE]);
	}
	double Run(const long long kITER, const long long kSIZE);
private:
	std::unique_ptr<Node[]> nodes_;
	void CreateRing(const long long kRANGE_NODE);
	Node* Traverse(Node *head, const long long kITER);
};

void LatencyBench::CreateRing(const long long kRANGE_NODE)
{
	if (kRANGE_NODE < 0 or (kRANGE_NODE&(kRANGE_NODE-1)) != 0) {
		abort();
	}
	std::default_random_engine rng;
	std::uniform_int_distribution<long> uid;
	nodes_[0].next = nodes_.get();
	for (int i = 1; i < kRANGE_NODE; ++i) {
		Node *cur = nodes_.get() + uid(rng) % (i - i%32 + 1);
		Node *nxt = cur->next;
		Node *tail = nodes_.get() + i;
		cur->next = tail;
		tail->next = nxt;
	}
}

LatencyBench::Node* LatencyBench::Traverse(Node *head, const long long kITER)
{
	for (long long i = 0; i < kITER; ++i) {
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

double LatencyBench::Run(const long long kITER, const long long kRANGE_BYTE)
{
	if (kRANGE_BYTE > kNUM_BYTE) {
		abort();
	}
	CreateRing(kRANGE_BYTE / sizeof(Node));

	Timer timer;
	if (Traverse(nodes_.get(), kRANGE_BYTE / sizeof(Node)) == nullptr) { abort(); }
	timer.Start();
	if (Traverse(nodes_.get(), kITER) == nullptr) { abort(); }
	timer.Pause();
	return double(timer.get_count()) * 1000 / (kITER * 8);
}

int main()
{
	LatencyBench bench(1ll<<30);
	for (long long sz = 1ll<<10; sz <= 1ll<<26; sz *= 2) {
		std::cout << sz << " " << bench.Run(1ll<<26, sz) << std::endl;
	}
	return 0;
}