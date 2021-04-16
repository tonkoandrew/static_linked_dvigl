#include "spdlog/spdlog.h"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_net.h"
#include "SDL_ttf.h"

#include <assimp/cexport.h> // C exporter interface
#include <assimp/cimport.h> // C importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h> // Output data structure

#include <yaml-cpp/yaml.h>


#include "mainwindow.h"
#include <QApplication>


char* get_content(std::string file_name)
{
    Sint64 bufferLength = 0;

    SDL_RWops* rw = SDL_RWFromFile(file_name.c_str(), "rb");
    if (rw != NULL)
    {
        /* Seek to 0 bytes from the end of the file */
        bufferLength = SDL_RWsize(rw);
        char* buffer = NULL;
        if (bufferLength == 0)
        {
            spdlog::error("File is empty: {}", file_name);
        }
        else
        {
            SDL_RWseek(rw, 0, RW_SEEK_SET);
            buffer = (char*)malloc(bufferLength + 1);
            if (buffer != NULL)
            {
                SDL_RWread(rw, buffer, bufferLength, 1);
                buffer[bufferLength] = '\0';
            }
            else
            {
	            spdlog::error("MEMORY ALLOCATION ERROR for file: {}", file_name);
                return NULL;
            }
        }
        SDL_RWclose(rw);
        return buffer;
    }
    else
    {
	    spdlog::error("ERROR LOADING FILE {} - file not opened", file_name);
        return NULL;
    }
}

int get_size(std::string file_name)
{
    int size = 0;
    SDL_RWops* rw = SDL_RWFromFile(file_name.c_str(), "rb");
    if (rw != NULL)
    {
        size = SDL_RWsize(rw);
        SDL_RWclose(rw);
    }
    else
    {
        spdlog::error("File not exist: {}", file_name);
    }
    return size;
}

int main(int argc, char* argv[])
{
	spdlog::set_level(spdlog::level::debug);
	spdlog::info("Hello editor");


	char* content = get_content("../res/models/elvis.dae");
    int content_size = get_size("../res/models/elvis.dae");
    float scale = 1.0f;

    std::string format = "collada";

	const struct aiScene* scene;

    int flags = aiProcess_SplitLargeMeshes | aiProcess_LimitBoneWeights | aiProcess_SplitByBoneCount
        | aiProcess_GenSmoothNormals | aiProcess_SortByPType | aiProcess_CalcTangentSpace
        | aiProcess_RemoveRedundantMaterials | aiProcess_TransformUVCoords | aiProcess_ValidateDataStructure
        | aiProcess_ImproveCacheLocality | aiProcess_FindInvalidData | aiProcess_FindDegenerates
        | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_Triangulate | aiProcess_FlipUVs
        | aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_GlobalScale | 0;

    aiPropertyStore* store = aiCreatePropertyStore();
    aiSetImportPropertyFloat(store, AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, scale);

    int import_components = aiComponent_CAMERAS | aiComponent_LIGHTS | aiComponent_MATERIALS | aiComponent_COLORS;
    aiSetImportPropertyInteger(store, AI_CONFIG_PP_RVC_FLAGS, import_components);

    scene = aiImportFileFromMemoryWithProperties(content, content_size, flags, format.c_str(), store);
    aiReleasePropertyStore(store);

    if (!scene)
    {
        spdlog::error("ERROR ON LOADING MODEL");
        return 1;
    }

    spdlog::debug("scene loaded, mNumMeshes is {}", scene->mNumMeshes);


    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    return a.exec();
}