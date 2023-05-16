void readSubgroupsXML(tinyxml2::XMLElement *subgroupXML, std::vector<DrawModel> &sceneDrawModels, std::vector<Transform*> transf);
void readGroupXML(tinyxml2::XMLElement *groupXML, std::vector<DrawModel> &sceneDrawModels);
void readXML(char* filename, Scene &mainScene);