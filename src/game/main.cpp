__declspec(dllimport) int entrypoint(int _argc, char const *_argv[]);

int main(int argc, char const *argv[])
{
	return entrypoint(argc, argv);
}
