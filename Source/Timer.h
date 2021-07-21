/* Timer counts the time spent during the algorithm's execution.*/

class Timer {
	private:
		std::string funcao;
		clock_t inicio;
	public:
		Timer(const std::string &f) {
			inicio = clock();
			funcao = f;
	}
	~MedeTempo() {
		clock_t total = clock() - inicio;
		cout << "Tempo para a funcao " << funcao << ": (segundos) "<< double(total)/CLOCKS_PER_SEC<<endl;
	}
};
