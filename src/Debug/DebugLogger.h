#pragma once

#include <string>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <regex>
#include <iostream>
#include <array>


#ifdef _DEBUG
constexpr bool DEBUG_ENABLED = true;
#else
constexpr bool DEBUG_ENABLED = false;
#endif

struct FUNCTION{};
struct CONSTRUCTOR{};

class DebugLoggerInstanceFake {
public:

	static void breakAtMessage(size_t id) {}
	static void breatAtObject(size_t id) {}
	static void breakOnMessageName(std::string name) {}

	static void printOrderedByClass() {}
	static void printOrderedByObject() {}
	static void printOrderedByMessage() {}
	static void printOrderedByNamedObjects() {}
	static void printOrderedByNamedMessages() {}

	static void printClass(std::string key) {}
	static void printClass(size_t key) {}

	static void setObjectMessageLimit(size_t key, size_t limit) {}
	static void setClassMessageLimit(size_t key, size_t limit) {}
	static void setObjectAccessLimit(size_t key, size_t limit) {}
	static void setClassAccessLimit(size_t key, size_t limit) {}

	static void setObjectMessageLimit(std::string name, size_t limit) {}
	static void setClassMessageLimit(std::string name, size_t limit) {}
	static void setObjectAccessLimit(std::string name, size_t limit) {}
	static void setClassAccessLimit(std::string name, size_t limit) {}

	static void setObjectNameMessageLimit(std::string name, size_t limit) {}
	static void setMessageNameMessageLimit(std::string name, size_t limit) {}

	static void setDefaultObjectMessageLimit(size_t limit) {}
	static void setDefaultClassMessageLimit(size_t limit) {}
	static void setDefaultObjectAccessLimit(size_t limit) {}
	static void setDefaultClassAccessLimit(size_t limit) {}

	static void setDefaultObjectNameMessageLimit(size_t limit) {}
	static void setDefaultMessageNameMessageLimit(size_t limit) {}

	static void disableLogging() {}
	static void enableLogging() {}

};

template<class DTS = DebugTrackingStructure>
struct CountsAndLimits {
private:
	//typedef DebugTrackingStructure DTS;
public:
	CountsAndLimits(size_t classKey, size_t objectKey, size_t messageKey) : classKey(classKey), objectKey(objectKey), messageKey(messageKey){
		objectMessageCount = DTS::getCount(DTS::objectMapWithMessagePrintCount, objectKey);
		objectMessageLimit = DTS::getLimit(DTS::objectMapWithMessagePrintLimit, objectKey);

		classMessageCount = DTS::getCount(DTS::classMapWithMessagePrintCount, classKey);
		classMessageLimit = DTS::getLimit(DTS::classMapWithMessagePrintLimit, classKey);

		objectAccessCount = DTS::getCount(DTS::objectMapWithAccessCount, objectKey);
		objectAccessLimit = DTS::getLimit(DTS::objectMapWithAccessLimit, objectKey);

		classAccessCount = DTS::getCount(DTS::classMapWithAccessCount, classKey);
		classAccessLimit = DTS::getLimit(DTS::classMapWithAccessLimit, classKey);

		objectNameMessageCount = DTS::getCount(DTS::objectNameMapWithMessageCount,  DTS::getObjectNameFromObject(objectKey));
		objectNameMessageLimit = DTS::getLimit(DTS::objectNameMapWithMessageLimit, DTS::getObjectNameFromObject(objectKey));

		messageNameMessageCount = DTS::getCount(DTS::messageNameMapWithMessageCount, DTS::getMessageNameFromMessage(messageKey));
		messageNameMessageLimit = DTS::getLimit(DTS::messageNameMapWithMessageLimit, DTS::getMessageNameFromMessage(messageKey));
	}

	void incrementAndUpdate() {
		DTS::setCount(DTS::objectMapWithMessagePrintCount, objectKey, ++objectMessageCount);
		DTS::setCount(DTS::classMapWithMessagePrintCount, classKey, ++classMessageCount);
		DTS::setCount(DTS::objectMapWithAccessCount, objectKey, ++objectAccessCount);
		DTS::setCount(DTS::classMapWithAccessCount, classKey, ++classAccessCount);

		DTS::setCount(DTS::objectNameMapWithMessageCount, DTS::getObjectNameFromObject(objectKey), ++objectNameMessageCount);
		DTS::setCount(DTS::messageNameMapWithMessageCount, DTS::getMessageNameFromMessage(messageKey), ++messageNameMessageCount);
	}

	bool checkValidity() {
		return checkClassValidity() && checkObjectValidity() && checkMessageValidity();
	}

	bool checkClassValidity() {
		return DTS::checkCountMapWithin(classMessageCount, classMessageLimit, DTS::defaultClassMessagePrintLimit) &&
			DTS::checkCountMapWithin(classAccessCount, classAccessLimit, DTS::defaultClassAccessLimit);
	}
	
	bool checkObjectValidity() {
		return DTS::checkCountMapWithin(objectMessageCount, objectMessageLimit, DTS::defaultObjectMessagePrintLimit) &&
			DTS::checkCountMapWithin(objectAccessCount, objectAccessLimit, DTS::defaultObjectAccessLimit) &&
			DTS::checkCountMapWithin(objectNameMessageCount, objectNameMessageLimit, DTS::defaultObjectNameMapWithMessageLimit);
	}

	bool checkMessageValidity() {
		return DTS::checkCountMapWithin(messageNameMessageCount, messageNameMessageLimit, DTS::defaultMessageNameMapWithMessageLimit);
	}

private:
	size_t messageKey;
	size_t classKey;
	size_t objectKey;

	size_t objectMessageCount = 0;
	size_t objectMessageLimit = 0;

	size_t classMessageCount = 0;
	size_t classMessageLimit = 0;

	size_t objectAccessCount = 0;
	size_t objectAccessLimit = 0;

	size_t classAccessCount = 0;
	size_t classAccessLimit = 0;

	size_t objectNameMessageCount = 0;
	size_t objectNameMessageLimit = 0;

	size_t messageNameMessageCount = 0;
	size_t messageNameMessageLimit = 0;
	
};

struct PrintStructure {
public:
	PrintStructure() {}
	PrintStructure(std::string message) : message(message) {}
	PrintStructure(size_t classKey, size_t objectKey, size_t messageKey);
	PrintStructure(std::string message, size_t classKey, size_t objectKey, size_t messageKey);

	void initialization(size_t classKey, size_t objectKey, size_t messageKey);

	std::string message;
	size_t messageKey = 0;
	size_t objectKey = 0;
	size_t perClassObjectKey = 0;
	size_t classKey = 0;
	size_t perClassMessageKey = 0;
	size_t perObjectMessageKey = 0;
	std::string objectName = "";
	std::string messageName = "";
	std::string className = "";
};

struct DebugTrackingStructure {
public:

	static void getMessagesFromKey(std::vector<PrintStructure>& messages, size_t key) {

		size_t objectKey = getObjectFromMessage(key);
		size_t classKey = getClassFromObject(objectKey);

		CountsAndLimits<> cnl(classKey, objectKey, key);

		if (cnl.checkValidity()) {
			absoluteMessageCount++;

			PrintStructure value(classKey, objectKey, key);
			messages.push_back(value);

			cnl.incrementAndUpdate();

			if (!cnl.checkClassValidity()) {
				auto classMap_it = --classMapWithObjectKeys.upper_bound(classKey);
				auto objectMap_it = --objectMapWithMessageKeys.upper_bound(classMap_it->second);
				auto message_it = DebugTrackingStructure::messageKeyWithMessage.find(objectMap_it->second);
				std::string message = getMessageContentFromMessageKey(message_it->first);
				PrintStructure value("...\t" + message + "\n", classMap_it->first, objectMap_it->first, message_it->first);
				messages.push_back(value);
			}
			else if (!cnl.checkObjectValidity()) {
				auto objectMap_it = --objectMapWithMessageKeys.upper_bound(objectKey);
				auto message_it = DebugTrackingStructure::messageKeyWithMessage.find(objectMap_it->second);
				std::string message = getMessageContentFromMessageKey(message_it->first);
				PrintStructure value("...\t" + message + "\n", classKey, objectMap_it->first, message_it->first);
				messages.push_back(value);
			}
		}

	}

	static void getMessagesFromObject(std::vector<PrintStructure>& messages, size_t key) {

		size_t classKey = getClassFromObject(key);
		auto keys_it = objectMapWithMessageKeys.equal_range(key);
		for (auto it = keys_it.first; (it != keys_it.second); it++) {
			getMessagesFromKey(messages, it->second);
		}
	}

	static void getMessagesFromClass(std::vector<PrintStructure>& messages, size_t key) {
		CountsAndLimits<> cnl(key, 0, 0);
		auto keys_it = classMapWithObjectKeys.equal_range(key);
		for (auto it = keys_it.first; (it != keys_it.second); it++) {
			getMessagesFromObject(messages, it->second);
		}
	}

	static void getMessagesFromNamedObjects(std::vector<PrintStructure>& messages, std::string name) {
		auto keys_it = objectNameMapWithObjects.equal_range(name);
		for (auto it = keys_it.first; (it != keys_it.second); it++) {
			getMessagesFromObject(messages, it->second);
		}

	}

	static void getMessagesFromNamedMessages(std::vector<PrintStructure>& messages, std::string name) {
		auto keys_it = messageNameMapWithMessages.equal_range(name);
		for (auto it = keys_it.first; (it != keys_it.second); it++) {
			getMessagesFromKey(messages, it->second);
		}

	}

	//Storage and Structure of messages, objects and classes.
	inline static std::map<size_t, size_t> messageKeyWithMessage;
	inline static std::multimap<size_t, size_t> objectMapWithMessageKeys;
	inline static std::multimap<size_t, size_t> classMapWithObjectKeys;
	
	inline static std::multimap<std::string, size_t> objectNameMapWithObjects;
	inline static std::multimap<std::string, size_t> messageNameMapWithMessages;
	

	//Convertion/utility maps.
	inline static std::unordered_map<size_t, size_t> objectMapWithClassKeys;
	inline static std::unordered_map<size_t, std::string> classMapWithClassName;
	inline static std::unordered_map<std::string, size_t> classNameMapWithClass;
	inline static std::unordered_map<size_t, size_t> objectMapWithObjectInstance;
	inline static std::unordered_map<size_t, size_t> messageMapWithMessagePerClassInstance;
	inline static std::unordered_map<size_t, size_t> messageMapWithMessagePerObjectInstance;
	inline static std::unordered_map<size_t, size_t> messageMapWithObjectKeys;

	inline static std::unordered_map<size_t, std::string> messageMapWithMessageContent;
	inline static std::unordered_map<std::string, size_t> messageContentWithMessage;

	//Named maps
	inline static std::unordered_map <size_t, std::string> objectMapWithName;
	inline static std::unordered_map <size_t, std::string> messageMapWithName;

	//Counts of messages printed and object and classes accessed.
	inline static size_t absoluteMessageCount;
	inline static std::unordered_map<size_t, size_t> objectMapWithMessagePrintCount;
	inline static std::unordered_map<size_t, size_t> objectMapWithAccessCount;
	inline static std::unordered_map<size_t, size_t> classMapWithMessagePrintCount;
	inline static std::unordered_map<size_t, size_t> classMapWithAccessCount;

	inline static std::unordered_map<std::string, size_t> objectNameMapWithMessageCount;
	inline static std::unordered_map<std::string, size_t> messageNameMapWithMessageCount;


	inline static std::unordered_map<size_t, size_t> objectMapWithMessagePrintLimit;
	inline static std::unordered_map<size_t, size_t> objectMapWithAccessLimit;
	inline static std::unordered_map<size_t, size_t> classMapWithMessagePrintLimit;
	inline static std::unordered_map<size_t, size_t> classMapWithAccessLimit;

	inline static std::unordered_map<std::string, size_t> objectNameMapWithMessageLimit;
	inline static std::unordered_map<std::string, size_t> messageNameMapWithMessageLimit;

	inline static size_t defaultObjectMessagePrintLimit = SIZE_MAX;
	inline static size_t defaultClassMessagePrintLimit = SIZE_MAX;
	inline static size_t defaultObjectAccessLimit = SIZE_MAX;
	inline static size_t defaultClassAccessLimit = SIZE_MAX;

	inline static size_t defaultObjectNameMapWithMessageLimit = SIZE_MAX;
	inline static size_t defaultMessageNameMapWithMessageLimit = SIZE_MAX;

	template<class Key>
	static size_t getCount(std::unordered_map<Key, size_t>& umap, Key key) {
		auto it = umap.find(key);
		if (it != umap.end()) {
			return it->second;
		}
		else {
			return 0;
		}
	}

	template<class Key>
	static size_t getLimit(std::unordered_map<Key, size_t>& umap, Key key) {
		auto it = umap.find(key);
		if (it != umap.end()) {
			return it->second;
		}
		else {
			return SIZE_MAX;
		}
	}

	static void resetCounts() {
		absoluteMessageCount = 0;
		objectMapWithMessagePrintCount.clear();
		objectMapWithAccessCount.clear();
		classMapWithMessagePrintCount.clear();
		classMapWithAccessCount.clear();
	}

	template<class Key>
	static void setCount(std::unordered_map<Key, size_t>& umap, Key key, size_t newCount) {
		auto it = umap.find(key);
		if (it == umap.end()) {
			it = umap.insert({ key, newCount }).first;
		}
		else {
			it->second = newCount;
		}
	}

	static std::string getMessageContentFromMessageKey(size_t key) {
		auto messageKey_it = messageKeyWithMessage.find(key);
		if (messageKey_it != messageKeyWithMessage.end()) {
			return messageMapWithMessageContent.find(messageKey_it->second)->second;
		}
		else {
			return "";
		}
	}

	static std::string getClassNameFromClass(size_t key) {
		auto name = classMapWithClassName.find(key);
		if (name != classMapWithClassName.end()) {
			return name->second;
		}
		else {
			return "";
		}
	}

	static size_t getClassFromClassName(std::string name) {
		auto it = classNameMapWithClass.find(name);
		if (it != classNameMapWithClass.end()) {
			return it->second;
		}
		else {
			return 0;
		}
	}

	static std::string getObjectNameFromObject(size_t key) {
		auto name = objectMapWithName.find(key);
		if (name != objectMapWithName.end()) {
			return name->second;
		}
		else {
			return "";
		}
	}

	static std::string getMessageNameFromMessage(size_t key) {
		auto name = messageMapWithName.find(key);
		if (name != messageMapWithName.end()) {
			return name->second;
		}
		else {
			return "";
		}
	}

	static size_t getObjectFromMessage(size_t key) {
		return messageMapWithObjectKeys.find(key)->second;
	}

	static size_t getClassInstanceFromObject(size_t key) {
		return objectMapWithObjectInstance.find(key)->second;
	}

	static size_t getClassFromObject(size_t key) {
		return objectMapWithClassKeys.find(key)->second;
	}

	static bool checkCountMapWithin(size_t count, size_t limit, size_t defaultLimit) {
		if (limit == SIZE_MAX) {
			limit = defaultLimit;
		}
		return (limit > count);
	}
};

class DebugLoggerInstance : protected DebugTrackingStructure {
public:
	DebugLoggerInstance();
	DebugLoggerInstance(DebugLoggerInstance& other);
	DebugLoggerInstance(DebugLoggerInstance&& other);

	static void breakAtMessage(size_t id);
	static void breatAtObject(size_t id);
	static void breakOnMessageName(std::string name);

	static void printOrderedByClass();
	static void printOrderedByObject();
	static void printOrderedByMessage();
	static void printOrderedByNamedObjects();
	static void printOrderedByNamedMessages();

	static void printClass(std::string key);
	static void printClass(size_t key);

	static void setObjectMessageLimit(size_t key, size_t limit);
	static void setClassMessageLimit(size_t key, size_t limit);
	static void setObjectAccessLimit(size_t key, size_t limit);
	static void setClassAccessLimit(size_t key, size_t limit);

	static void setObjectMessageLimit(std::string name, size_t limit);
	static void setClassMessageLimit(std::string name, size_t limit);
	static void setObjectAccessLimit(std::string name, size_t limit);
	static void setClassAccessLimit(std::string name, size_t limit);

	static void setObjectNameMessageLimit(std::string name, size_t limit);
	static void setMessageNameMessageLimit(std::string name, size_t limit);

	static void setDefaultObjectMessageLimit(size_t limit);
	static void setDefaultClassMessageLimit(size_t limit);
	static void setDefaultObjectAccessLimit(size_t limit);
	static void setDefaultClassAccessLimit(size_t limit);

	static void setDefaultObjectNameMessageLimit(size_t limit);
	static void setDefaultMessageNameMessageLimit(size_t limit);

	static void disableLogging();
	static void enableLogging();

protected:
	template<class Type>
	static std::string getClassName() {
		std::regex reg("\\w+\\s+([\\w\\W]+)");
		std::smatch match;
		std::string str = typeid(Type).name();
		std::regex_search(str, match, reg);

		return match[1].str();
	}

	inline static bool loggingEnabled = true;

private:
	static void print(std::vector<PrintStructure>& messages);

protected:

	inline static size_t absoluteObjectInstanceCount = 0;
	const size_t absoluteObjectInstance = 0;

	inline static std::unordered_map<std::string, size_t> classNameMapWithClassKey;

	inline static std::unordered_set<size_t> objectKeySetBreakPoints;
	inline static std::unordered_set<size_t> messageKeySetBreakPoints;
	inline static std::unordered_set<std::string> messageNameSetBreakPoints;
};

class _NODEBUG_ : public DebugLoggerInstanceFake {
public:
	_NODEBUG_(std::string classLogName = "") {}
	_NODEBUG_(_NODEBUG_&& other) {}

	void debug(std::string& message, std::string& log) {}
	void debug(std::string&& message, std::string&& log) {}
	void debug(std::string& message, std::string&& log) {}
	void debug(std::string&& message, std::string& log) {}
	void debug(std::string message) {}
	void setThisLimit(size_t& limit) {}
	void setThisMessageLimit(size_t& limit) {}
	void setThisLimit(size_t&& limit) {}
	void setThisMessageLimit(size_t&& limit) {}
	size_t getObjectKey() const { return 0; }
};

template<class Class>
class _DebugLogger : public DebugLoggerInstance
{

public:
	_DebugLogger(std::string objectName = "") : classInstance(++classInstanceCount) {

		if (loggingEnabled) {
			//Set class key if not already.
			if (className == "") {
				className = getClassName<Class>();
			}
			auto classNameMap_it = classNameMapWithClassKey.find(className);
			if (classNameMap_it == classNameMapWithClassKey.end()) {
				classKey = classNameMapWithClassKey.size() + 1;
				classNameMapWithClassKey.insert({ className, classKey });
				classNameMapWithClass.insert({ className,classKey });
				classMapWithClassName.insert({ classKey, className });
			}
			else {
				classKey = classNameMap_it->second;
			}
			objectMapWithObjectInstance.insert({ absoluteObjectInstance, classInstance });
			classMapWithObjectKeys.insert({ classKey, absoluteObjectInstance });
			objectMapWithClassKeys.insert({ absoluteObjectInstance, classKey });
			if (objectName != "") {
				objectMapWithName.insert({ absoluteObjectInstance, objectName });
				objectNameMapWithObjects.insert({ objectName, absoluteObjectInstance });
			}

			if (objectKeySetBreakPoints.find(absoluteObjectInstance) != objectKeySetBreakPoints.end()) {
				throw;
			}
		}
	}
	_DebugLogger(_DebugLogger& other) : DebugLoggerInstance(std::move(other)) {}
	_DebugLogger(_DebugLogger&& other) : DebugLoggerInstance(std::move(other)) {}

	void debug(std::string message, std::string messageName = "")
	{
		if (loggingEnabled) {
			classMessageCount++;
			classMessage++;
			size_t messageKey = DebugTrackingStructure::messageKeyWithMessage.size() + 1;
			size_t contentKey = DebugTrackingStructure::messageContentWithMessage.size() + 1;

			auto mCWM_IT = DebugTrackingStructure::messageContentWithMessage.insert({ message, contentKey }).first;
			DebugTrackingStructure::messageMapWithMessageContent.insert({ mCWM_IT->second, message });
			DebugTrackingStructure::messageKeyWithMessage.insert({ messageKey, mCWM_IT->second });

			objectMapWithMessageKeys.insert({ absoluteObjectInstance, messageKey });
			messageMapWithObjectKeys.insert({ messageKey,absoluteObjectInstance });

			messageMapWithMessagePerClassInstance.insert_or_assign(messageKey, classMessageCount);
			messageMapWithMessagePerObjectInstance.insert_or_assign(messageKey, classMessage);
			if (messageName != "") {
				messageMapWithName.insert({ messageKey, messageName });
				messageNameMapWithMessages.insert({ messageName, messageKey });
			}


			if (messageKeySetBreakPoints.find(messageKey) != messageKeySetBreakPoints.end() ||
				messageNameSetBreakPoints.find(messageName) != messageNameSetBreakPoints.end()) {
				throw;
			}
		}
	}

	void setThisLimit(size_t limit)
	{
		objectMapWithAccessLimit.insert_or_assign(absoluteObjectInstance, limit);
	}

	void setThisMessageLimit(size_t limit)
	{
		objectMapWithMessagePrintLimit.insert_or_assign(absoluteObjectInstance, limit);
	}

	size_t getObjectKey() const{
		return absoluteObjectInstance;
	}


private:

	//Count of instances per class this is inherited into. Defined by template parameters.
	inline static size_t classInstanceCount = 0;
	inline static size_t classMessageCount = 0;
	//Enumeration of this object per class this is inherited into. Defined by template parameters.
	const size_t classInstance = 0;
	size_t classMessage = 0;

	inline static std::string className = "";
	inline static size_t classKey;

};

template<class Class = void>
using DebugLogger = std::conditional_t<DEBUG_ENABLED, _DebugLogger<Class>, _NODEBUG_>;


//Get adresses for all objects.
//Throw on messages with [name]