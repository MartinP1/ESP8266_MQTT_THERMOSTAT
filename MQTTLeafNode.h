class CMQTTLeafNode {
    private:
       String m_strPath;
       String m_strLastVal;
       void (*m_pCallback)(const char*);
    public: 
        CMQTTLeafNode(){
          m_pCallback=NULL;
        } 
        /**
        * Function SetPath
        * 
        * @param path[in]
        */
       void SetBinding(const char* path, void (*callback)(const char*)) {
           m_strPath = path;
           m_pCallback = callback;
       }
       void Publish (float fValue) {
          m_strLastVal = String(fValue);
       if (mqttClient.connected())
          mqttClient.publish(m_strPath.c_str(), 0, true, m_strLastVal.c_str());
        delay(10);  
       }
       bool TestReceived(const char*topic, const char *payload){
        if (m_strPath.length()<3)
           return false;
        if (m_strPath!=topic)
          return false;
        m_strLastVal=payload;
        if (m_pCallback) m_pCallback(payload);
       }
    private:
};