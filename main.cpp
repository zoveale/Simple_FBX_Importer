#include "imfbx.h"

int main(int argc, char* argv[]) {

  Simflex StatueScene;
  StatueScene.FileImport("statue.fbx");
  StatueScene.PrintAllNodes();

  return 0;
}