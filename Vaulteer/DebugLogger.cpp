#include "vpch.h"
#include "DebugLogger.h"


DebugLoggerInstance::DebugLoggerInstance() : absoluteObjectInstance(++absoluteObjectInstanceCount)
{
}

DebugLoggerInstance::DebugLoggerInstance(DebugLoggerInstance& other) : absoluteObjectInstance(++absoluteObjectInstanceCount)
{
}

DebugLoggerInstance::DebugLoggerInstance(DebugLoggerInstance&& other) : absoluteObjectInstance(other.absoluteObjectInstance)
{
}

void DebugLoggerInstance::breakAtMessage(size_t id)
{
	messageKeySetBreakPoints.insert(id);
}

void DebugLoggerInstance::breatAtObject(size_t id)
{
	objectKeySetBreakPoints.insert(id);
}

void DebugLoggerInstance::breakOnMessageName(std::string name)
{
	messageNameSetBreakPoints.insert(name);
}

void DebugLoggerInstance::printOrderedByClass() {
	resetCounts();
	std::vector<PrintStructure> vMessages;
	for (auto it = classMapWithObjectKeys.begin(); it != classMapWithObjectKeys.end(); it = classMapWithObjectKeys.upper_bound(it->first)) {
		
		getMessagesFromClass(vMessages, it->first);

	}
	print(vMessages);
}
void DebugLoggerInstance::printOrderedByObject() {
	resetCounts();
	std::vector<PrintStructure> vMessages;
	for (auto it = objectMapWithMessageKeys.begin(); it != objectMapWithMessageKeys.end(); it = objectMapWithMessageKeys.upper_bound(it->first)) {

		getMessagesFromObject(vMessages, it->first);

	}
	print(vMessages);
}
void DebugLoggerInstance::printOrderedByMessage() {
	resetCounts();
	std::vector<PrintStructure> vMessages;
	for (auto it = messageKeyWithMessage.begin(); it != messageKeyWithMessage.end(); it++) {

		getMessagesFromKey(vMessages, it->first);

	}
	print(vMessages);
}

void DebugLoggerInstance::printOrderedByNamedObjects() {
	resetCounts();
	std::vector<PrintStructure> vMessages;
	for (auto it = objectNameMapWithObjects.begin(); it != objectNameMapWithObjects.end(); it = objectNameMapWithObjects.upper_bound(it->first)) {

		getMessagesFromNamedObjects(vMessages, it->first);

	}
	print(vMessages);
}

void DebugLoggerInstance::printOrderedByNamedMessages() {
	resetCounts();
	std::vector<PrintStructure> vMessages;
	for (auto it = messageNameMapWithMessages.begin(); it != messageNameMapWithMessages.end(); it = messageNameMapWithMessages.upper_bound(it->first)) {

		getMessagesFromNamedMessages(vMessages, it->first);

	}
	print(vMessages);
}

void DebugLoggerInstance::printClass(std::string key) {
	std::vector<PrintStructure> vMessages;
	getMessagesFromClass(vMessages, getClassFromClassName(key));
	print(vMessages);
}
void DebugLoggerInstance::printClass(size_t key) {
	std::vector<PrintStructure> vMessages;
	getMessagesFromClass(vMessages, key);
	print(vMessages);
}

void DebugLoggerInstance::print(std::vector<PrintStructure>& messages) {
	std::vector<std::string> info;
	std::vector<std::string> mess;
	std::vector<size_t> size;
	size_t maxInfoSize = 0;
	for (const auto& msg : messages) {
		std::string namedMessage = "";
		std::string namedObject = "";
		std::string namedClass = "";
		if (msg.className != "") {
			namedClass = "(" + msg.className + ")";
		}
		if (msg.objectName != "") {
			namedObject = "(" + msg.objectName + ")";
		}
		if (msg.objectName != "") {
			namedMessage = "(" + msg.messageName + ")";
		}
		mess.push_back(msg.message);
		info.push_back(
			"[" + std::to_string(msg.classKey) + "]" + namedClass + 
			"[" + std::to_string(msg.objectKey) + ", " + std::to_string(msg.perClassObjectKey) + "]" + namedObject + 
			"[" + std::to_string(msg.messageKey) + ", " + std::to_string(msg.perClassMessageKey) + ", " + std::to_string(msg.perObjectMessageKey) + "]" + namedMessage + "  ");
		size.push_back(info.back().size());
		if (maxInfoSize < info.back().size()) {
			maxInfoSize = info.back().size();
		}
		
		//std::cout << "[" << namedClass << msg.classKey << "][" << namedObject << msg.objectKey << ", " << msg.perClassObjectKey << "][" << namedMessage << msg.messageKey << ", " << msg.perClassMessageKey << ", " << msg.perObjectMessageKey << "] " << msg.message;
	}

	for (size_t i = 0; i < mess.size(); i++) {
		std::string append((maxInfoSize - size[i]), ' ');
		std::cout << info[i] << append << mess[i];
	}
}

PrintStructure::PrintStructure(size_t classKey, size_t objectKey, size_t messageKey) :
	classKey(classKey),
	objectKey(objectKey),
	messageKey(messageKey) {

	message = DebugTrackingStructure::getMessageContentFromMessageKey(messageKey);
	initialization(classKey, objectKey, messageKey);
}

PrintStructure::PrintStructure(std::string message, size_t classKey, size_t objectKey, size_t messageKey) :
	classKey(classKey),
	objectKey(objectKey),
	messageKey(messageKey),
	message(message)
{
	initialization(classKey, objectKey, messageKey);
}

void PrintStructure::initialization(size_t classKey, size_t objectKey, size_t messageKey)
{
	perClassObjectKey = DebugTrackingStructure::getClassInstanceFromObject(objectKey);
	perClassMessageKey = DebugTrackingStructure::messageMapWithMessagePerClassInstance.find(messageKey)->second;
	perObjectMessageKey = DebugTrackingStructure::messageMapWithMessagePerObjectInstance.find(messageKey)->second;
	objectName = DebugTrackingStructure::getObjectNameFromObject(objectKey);
	messageName = DebugTrackingStructure::getMessageNameFromMessage(messageKey);
	className = DebugTrackingStructure::getClassNameFromClass(classKey);
}

void DebugLoggerInstance::setObjectMessageLimit(size_t key, size_t limit) {
	objectMapWithMessagePrintLimit.insert_or_assign(key, limit);
}
void DebugLoggerInstance::setClassMessageLimit(size_t key, size_t limit) {
	classMapWithMessagePrintLimit.insert_or_assign(key, limit);
}
void DebugLoggerInstance::setObjectAccessLimit(size_t key, size_t limit) {
	objectMapWithAccessLimit.insert_or_assign(key, limit);
}
void DebugLoggerInstance::setClassAccessLimit(size_t key, size_t limit) {
	classMapWithAccessLimit.insert_or_assign(key, limit);
}

void DebugLoggerInstance::setObjectMessageLimit(std::string name, size_t limit) {
	auto keys_it = objectNameMapWithObjects.equal_range(name);
	for (auto it = keys_it.first; (it != keys_it.second); it++) {
		objectMapWithMessagePrintLimit.insert_or_assign(it->second, limit);
	}
}
void DebugLoggerInstance::setClassMessageLimit(std::string name, size_t limit) {
	classMapWithMessagePrintLimit.insert_or_assign(classNameMapWithClassKey.find(name)->second, limit);
}
void DebugLoggerInstance::setObjectAccessLimit(std::string name, size_t limit) {
	auto keys_it = objectNameMapWithObjects.equal_range(name);
	for (auto it = keys_it.first; (it != keys_it.second); it++) {
		objectMapWithAccessLimit.insert_or_assign(it->second, limit);
	}
}
void DebugLoggerInstance::setClassAccessLimit(std::string name, size_t limit) {
	classMapWithAccessLimit.insert_or_assign(classNameMapWithClassKey.find(name)->second, limit);
}

void DebugLoggerInstance::setObjectNameMessageLimit(std::string name, size_t limit) {
	objectNameMapWithMessageLimit.insert_or_assign(name, limit);
}
void DebugLoggerInstance::setMessageNameMessageLimit(std::string name, size_t limit) {
	messageNameMapWithMessageLimit.insert_or_assign(name, limit);
}

void DebugLoggerInstance::setDefaultObjectMessageLimit(size_t limit) {
	defaultObjectMessagePrintLimit = limit;
}
void DebugLoggerInstance::setDefaultClassMessageLimit(size_t limit) {
	defaultClassMessagePrintLimit = limit;
}
void DebugLoggerInstance::setDefaultObjectAccessLimit(size_t limit) {
	defaultObjectAccessLimit = limit;
}
void DebugLoggerInstance::setDefaultClassAccessLimit(size_t limit) {
	defaultClassAccessLimit = limit;
}

void DebugLoggerInstance::setDefaultObjectNameMessageLimit(size_t limit) {
	defaultObjectNameMapWithMessageLimit = limit;
}
void DebugLoggerInstance::setDefaultMessageNameMessageLimit(size_t limit) {
	defaultMessageNameMapWithMessageLimit = limit;
}

void DebugLoggerInstance::disableLogging() {
	loggingEnabled = false;
}
void DebugLoggerInstance::enableLogging() {
	loggingEnabled = true;
}