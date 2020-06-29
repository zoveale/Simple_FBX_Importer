#ifndef V_322DF86A_7BE_4050_B67F_85F28DC5A453
#define V_322DF86A_7BE_4050_B67F_85F28DC5A453

#include <algorithm>

#include <fbxsdk.h>

class Simflex {
public:

  Simflex() {
    lSdkManager = FbxManager::Create();
    ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings((ios));
    lScene = nullptr;
    lRootNode = nullptr;
    numTabs = 0;
  }

  //include file extension ie: "example.fbx"
  Simflex(const char* filename) {
    lSdkManager = FbxManager::Create();
    ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings((ios));
    lScene = nullptr;
    lRootNode = nullptr;
    numTabs = 0;
    FileImport(filename);
  }

  //include file extension ie: "example.fbx"
  void FileImport(const char* filename) {
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
    // Use the first argument as the filename for the importer.
    if (!lImporter->Initialize(filename, -1, lSdkManager->GetIOSettings())) {
      printf("Call to FbxImporter::Initialize() failed.\n");
      printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
      exit(-1);
    }
    //initialize scene, import scene, destroy importer
    lScene = FbxScene::Create(lSdkManager, "myScene");
    lImporter->Import(lScene);
    lImporter->Destroy();

    //set root node
    lRootNode = lScene->GetRootNode();
  }

  void PrintAllNodes() {
    //check root node
    if (lRootNode) {
      for (int i = 0; i < lRootNode->GetChildCount(); i++)
        PrintNode(lRootNode->GetChild(i));
    }
    else {
      printf("Root node failed to initialize.\n");
      exit(-1);
    }
  }

  



  ~Simflex() {
    lSdkManager->Destroy();
  }
private:
  FbxManager* lSdkManager;
  FbxIOSettings* ios;
  FbxScene* lScene;
  FbxNode* lRootNode;

  //
  int numTabs;
  void PrintTabs() {
    for (int i = 0; i < numTabs; i++)
      printf("\t");
  }

  //helper functions
  FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
    switch (type) {
    case FbxNodeAttribute::eUnknown: return "unidentified";
    case FbxNodeAttribute::eNull: return "null";
    case FbxNodeAttribute::eMarker: return "marker";
    case FbxNodeAttribute::eSkeleton: return "skeleton";
    case FbxNodeAttribute::eMesh: return "mesh";
    case FbxNodeAttribute::eNurbs: return "nurbs";
    case FbxNodeAttribute::ePatch: return "patch";
    case FbxNodeAttribute::eCamera: return "camera";
    case FbxNodeAttribute::eCameraStereo: return "stereo";
    case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
    case FbxNodeAttribute::eLight: return "light";
    case FbxNodeAttribute::eOpticalReference: return "optical reference";
    case FbxNodeAttribute::eOpticalMarker: return "marker";
    case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
    case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
    case FbxNodeAttribute::eBoundary: return "boundary";
    case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
    case FbxNodeAttribute::eShape: return "shape";
    case FbxNodeAttribute::eLODGroup: return "lodgroup";
    case FbxNodeAttribute::eSubDiv: return "subdiv";
    default: return "unknown";
    }
  }

  void PrintAttribute(FbxNodeAttribute* pAttribute) {
    if (!pAttribute) return;

    FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
    FbxString attrName = pAttribute->GetName();
    PrintTabs();
    // Note: to retrieve the character array of a FbxString, use its Buffer() method.
    printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
  }

 
  void PrintNode(FbxNode* pNode) {
    PrintTabs();
    const char* nodeName = pNode->GetName();
    FbxDouble3 translation = pNode->LclTranslation.Get();
    FbxDouble3 rotation = pNode->LclRotation.Get();
    FbxDouble3 scaling = pNode->LclScaling.Get();

    // Print the contents of the node.
    printf("<node name='%s'\n translation='(%f, %f, %f)'\n rotation='(%f, %f, %f)'\n scaling='(%f, %f, %f)'>\n\n",
      nodeName,
      translation[0], translation[1], translation[2],
      rotation[0], rotation[1], rotation[2],
      scaling[0], scaling[1], scaling[2]
    );
    numTabs++;

    // Print the node's attributes.
    for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
      PrintAttribute(pNode->GetNodeAttributeByIndex(i));

    FbxMesh* pMesh = pNode->GetMesh();
   
    FbxVector4* lControlPoints = pMesh->GetControlPoints();
    unsigned int stride = 4;
    unsigned int vertexCount = pMesh->GetPolygonVertexCount();
    unsigned int polyCount = pMesh->GetPolygonCount();
    unsigned int pointCount = pMesh->GetControlPointsCount();
    unsigned int normalCount = pMesh->GetElementNormalCount();
    unsigned int uvCount = pMesh->GetElementUVCount();

    printf_s("Polygon count = %i\n", polyCount);
    printf_s("control point count = %i\n", pointCount);
    printf_s("vertex count = %i\n", vertexCount);
    printf_s("normal element count = %i\n", normalCount);
    printf_s("uv element count = %i\n", uvCount);

    
    for (unsigned int i = 0; i < pMesh->GetControlPointsCount(); ++i * stride) {
      
      printf_s("vertex[%i] = (%f, %f, %f)\n", i ,
              (float)lControlPoints[i][0],
              (float)lControlPoints[i][1],
              (float)lControlPoints[i][2]);
      FbxVector4 normals = pMesh->GetElementNormal()->GetDirectArray().GetAt(i);
      printf_s("normal[%i] = (%f, %f, %f)\n", i,
        (float)normals.mData[0],
        (float)normals.mData[1],
        (float)normals.mData[2]);

      FbxVector2 uv = pMesh->GetElementUV(0)->GetDirectArray().GetAt(i);
      
      printf_s("uv[%i] = (%f, %f)\n", i,
        (float)uv.mData[0],
        (float)uv.mData[1],
        (float)normals.mData[2]);
    }
    for (unsigned int i = 0; i < pMesh->GetPolygonCount(); ++i) {
      printf_s("polygon-triangle[%i] indices: ", i);
      for (unsigned int j = 0; j < 3; ++j) {
        printf_s("%i, ", pMesh->GetPolygonVertex(i, j));
      }
      printf_s("\n");
    }

    
    //Material
    printf_s("number of materials: %i\n", pNode->GetMaterialCount());
    
    for (unsigned int j = 0; j < pNode->GetMaterialCount(); ++j) {
      printf_s("materials[%i]: %s\n", j, pNode->GetMaterial(j)->GetName());
      printf_s("\ttype: %s\n", "");
    }
    pMesh->GetElementMaterial(0)->GetMappingMode();
    pMesh->GetElementMaterial(0)->GetReferenceMode();


    for (int j = 0; j < pNode->GetChildCount(); j++)
      PrintNode(pNode->GetChild(j));

    numTabs--;
    PrintTabs();
    printf_s("</node>\n");
  }
};
#endif // V_322DF86A_7BE_4050_B67F_85F28DC5A453

/*

*/