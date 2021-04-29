
extern "C" {
	__declspec(dllimport) int dvigl_init(int _argc, char const *_argv[]);
}

int main(int argc, char const *argv[])
{
	return dvigl_init(argc, argv);
}
