// API
//
//string::
//has_name
//clear_name
//const string& name()
//set_name(string)
//set_name(char*)
//
//int::
//has_name()
//clear_name()
//name()
//set_name()

//repeated files
//name_size() 
//add_name()
//
//
// serializes the message and stores the bytes in the given string. Note that the bytes are binary, not text; we only use the string class as a convenient container.
bool SerializeToString(string* output) const;
bool ParseFromString(const string& data);: parses a message from the given string.
bool SerializeToOstream(ostream* output) const;: writes the message to the given C++ ostream.
bool ParseFromIstream(istream* input);: parses a message from the given C++ istream.




// name
inline bool has_name() const;
inline void clear_name();
inline const ::std::string& name() const;
inline void set_name(const ::std::string& value);
inline void set_name(const char* value);
inline ::std::string* mutable_name();

//               // id
inline bool has_id() const;
inline void clear_id();
inline int32_t id() const;
inline void set_id(int32_t value);

//
// email
inline bool has_email() const;
inline void clear_email();
inline const ::std::string& email() const;
inline void set_email(const ::std::string& value);
inline void set_email(const char* value);
inline ::std::string* mutable_email();

// phone
inline int phone_size() const;
inline void clear_phone();
inline const ::google::protobuf::RepeatedPtrField< ::tutorial::Person_PhoneNumber >& phone() const;
inline ::google::protobuf::RepeatedPtrField< ::tutorial::Person_PhoneNumber >* mutable_phone();
inline const ::tutorial::Person_PhoneNumber& phone(int index) const;
inline ::tutorial::Person_PhoneNumber* mutable_phone(int index);
inline ::tutorial::Person_PhoneNumber* add_phone();


// Verify that the version of the library that we linked against is
//   // compatible with the version of the headers we compiled against.
GOOGLE_PROTOBUF_VERIFY_VERSION;

// Optional:  Delete all global objects allocated by libprotobuf.
google::protobuf::ShutdownProtobufLibrary();
