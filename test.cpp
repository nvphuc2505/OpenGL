#include <stdio.h>
#include <stdlib.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

int main() {
    // Load a model
    const char* filename = "path/to/your/model.obj"; // Thay đổi đường dẫn tới file model của bạn
    const aiScene* scene = aiImportFile(filename, aiProcess_Triangulate);

    if (!scene) {
        printf("Error loading model: %s\n", aiGetErrorString());
        return 1;
    }

    // Now you can access different parts of the loaded model through 'scene' variable

    // For example, printing the number of meshes in the model:
    printf("Number of meshes in the model: %d\n", scene->mNumMeshes);

    // Don't forget to free the scene
    aiReleaseImport(scene);

    return 0;
}
