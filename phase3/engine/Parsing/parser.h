void readSubgroupsXML(tinyxml2::XMLElement *subgroupXML, std::vector<Group> &subgroups, Scene &scene);
void readGroupXML(tinyxml2::XMLElement *group, Scene &scene);
void readXML(char* filename, Scene &scene);