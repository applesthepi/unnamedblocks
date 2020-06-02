#include "ProjectHandler.h"
#include "registries/BlockRegistry.h"
#include "stacking/Collection.h"
#include "stacking/Stack.h"
#include <SFML/System/Vector2.hpp>
#include <unordered_map>
#include <fstream>
#include <string_view>
#include <vector>
#include <cassert>
#include <bit>

//#ifdef POSIX
//#include <netinet/in.h>
//#else
//#include <WinSock2.h>
//#endif

// Oh hey a union
union u32 {
	uint32_t value;
	char bytes[sizeof(uint32_t)];
};
// More unions
union i32 {
	int32_t value;
	char bytes[sizeof(int32_t)];
};

union u16 {
	uint16_t value;
	char bytes[sizeof(uint16_t)];
};

union u8 {
	uint8_t value;
	char bytes[sizeof(uint8_t)];
};

union f32 {
	float value;
	char bytes[sizeof(float)];
};
inline void push(std::vector<char> &vec, u32 value) {
	u32 v = value;
	//v.value = htonl(v.value);
	for(char i : v.bytes) {
		vec.push_back(i);
	}
}
inline void push(std::vector<char> &vec, f32 value) {
	f32 v = value;
	//v.value = htonl(v.value);
	for(char i : value.bytes) {
		vec.push_back(i);
	}
}
inline void push(std::vector<char> &vec, i32 value) {
	i32 v = value;
	//v.value = htonl(v.value);
	for(char i : value.bytes) {
		vec.push_back(i);
	}
}
inline void push(std::vector<char> &vec, u16 value) {
	u16 v = value;
	//v.value = htons(v.value);
	for(char i : value.bytes) {
		vec.push_back(i);
	}
}
inline void push(std::vector<char> &vec, u8 value) {
	for(char i : value.bytes) {
		vec.push_back(i);
	}
}
inline void push(std::vector<char> &vec, std::string_view value) {
	for(char i : value) {
		vec.push_back(i);
	}
}
// Reads count elements and converts them to the native endian. Stores the result in buffer
// Ensure buffer is atleast size * count bytes
inline void read_16(std::ifstream& file, char* buffer, uint16_t count) {
	file.read(buffer, count * 2);
	for(uint16_t i = 0; i < count; i++) {
		//buffer[i] = ntohs(buffer[i * 2]);
	}
}
// Reads count elements and converts them to the native endian. Stores the result in buffer
// Ensure buffer is atleast size * count bytes
inline void read_32(std::ifstream& file, char* buffer, uint16_t count) {
	file.read(buffer, count * 4);
	for(uint16_t i = 0; i < count; i++) {
		//buffer[i] = ntohl(buffer[i * 4]);
	}
}
#define read_float read_32

// Layout
// Indent = Can repeat
// FIXME: Endianness
//
// (uint32_t) mod_count
//   (uint8_t) name_size (char[]) name
// (uint32_t) name_count
//   (uint8_t) name_size (char[]) name
//
// Stack:
// (uint32_t) collection_count
//   (float) collection_x collection_y (uint32_t) stack_count
//     (float) stack_x stack_y (uint32_t)block_count
//       (uint32_t) name_id (uint8_t) arg_count
//         (uint16_t) arg_size (char[]) arg
void ProjectHandler::SaveProject(const std::string& path) {
	std::unordered_map<std::string, uint32_t> mod_ids;
	std::unordered_map<const char*, uint32_t> block_ids;
	std::vector<std::string> mod_names;
	std::vector<std::string> block_names;
	uint32_t next_id = 0;
	const std::vector<Collection*>& collections = Plane::PrimaryPlane->GetCollections();

	//  Rough estimates
	std::vector<char> header;
	std::vector<char> body;
	header.reserve(1000 * collections.size());
	body.reserve(10000 * collections.size());

	push(body, u32 {static_cast<uint32_t>(collections.size())});
	// Collections
	for(Collection* i : collections) {
		const std::vector<Stack*>& stacks = i->GetStacks();
		push(body, f32 {i->getPosition().x});
		push(body, f32 {i->getPosition().y});
		push(body, u32 {static_cast<uint32_t>(stacks.size())});
		for(Stack* stack : stacks) {
			sf::Vector2f position = stack->getPosition();
			const std::vector<Block*>& blocks = stack->GetBlocks();

			push(body, f32 {position.x});
			push(body, f32 {position.y});
			push(body, u32 {static_cast<uint32_t>(blocks.size())});

			for(Block* block : blocks) {
				const char* block_name = block->GetModBlock()->GetUnlocalizedName();
				const std::string& mod_name = BlockRegistry::GetRegistry().GetBlockMod(block->GetModBlock());
				const std::vector<Argument*>& arguments = block->GetArguments();
				uint32_t id;

				// Set mod_name id if not already a thing
				if (mod_ids.contains(mod_name)) {
					id = mod_ids[mod_name];
				}
				else {
					id = next_id++;
					mod_ids[mod_name] = id;
					mod_names.push_back(mod_name);
				}

				// Set block_name id if not already a thing
				if (block_ids.contains(block_name)) {
					id = block_ids[block_name];
				}
				else {
					id = next_id++;
					block_ids[block_name] = id;
					block_names.push_back(block_name);
				}

				push(body, u32 {id});
				// Put placeholder argument count
				push(body, u8 {0});
				// Save the placeholder position for later
				const uint64_t pos = body.size();
				uint8_t argument_count = 0;
				for(Argument* argument : arguments) {
					if(!argument->HasData()) continue;
					argument_count++;
					const std::string& data = argument->GetData();
					// Check size
					UB_ASSERT(data.size() < 65535);

					push(body, u16 {(uint16_t)data.size()});
					push(body, data);
				}
				body[pos] = argument_count;
			}
		}
	}
	push(header, u32 {(uint32_t)mod_ids.size()});
	for(std::string& i : mod_names) {
		UB_ASSERT(i.size() < 255);
		push(header, u8 {(uint8_t)i.size()});
		push(header, i);
	}
	push(header, u32 {(uint32_t)block_ids.size()});
	for(std::string& i : block_names) {
		UB_ASSERT(i.size() < 255);
		push(header, u8 {(uint8_t)i.size()});
		push(header, i);
	}
	std::ofstream file(path, std::ofstream::trunc | std::ofstream::out | std::ofstream::binary);
	file.write(header.data(), static_cast<int64_t>(header.size()));
	file.write(body.data(), static_cast<int64_t>(body.size()));
	file.close();
}

// Loads a project from a file
void ProjectHandler::LoadProject(const std::string& path)
{
	// File read buffer
	char buffer[257];
	std::ifstream file(path, std::ifstream::in | std::ifstream::binary);

	if (!file.is_open())
	{
		Logger::Error("unable to open file \"" + path + "\"");
		return;
	}
	// Reset plane
	Plane::PrimaryPlane->SetInnerPosition(sf::Vector2i(0, 0));
	Plane::PrimaryPlane->DeleteContents();

	// Read mod header
	read_32(file, buffer, 1);

	uint32_t mod_name_count = *(uint32_t*)buffer;
	std::string* mod_names = new std::string[mod_name_count];
	for(uint32_t mod_name_index = 0; mod_name_index < mod_name_count; mod_name_index++) {
		uint8_t name_size = file.get();
		mod_names[mod_name_index].reserve(name_size);
		file.read(buffer, name_size);
		buffer[name_size] = 0;
		mod_names[mod_name_index] += buffer;
	}

	// Read block header
	read_32(file, buffer, 1);

	uint32_t block_name_count = *(uint32_t*)buffer;
	std::string* block_names = new std::string[block_name_count];
	for(uint32_t block_name_index = 0; block_name_index < block_name_count; block_name_index++) {
		uint8_t name_size = file.get();
		block_names[block_name_index].reserve(name_size);
		file.read(buffer, name_size);
		buffer[name_size] = 0;
		block_names[block_name_index] += buffer;
	}
	// Read collection count
	read_32(file, buffer, 1);
	uint32_t collection_count = *(uint32_t*)buffer;

	// Get collections
	for(uint32_t collection_index = 0; collection_index < collection_count; collection_index++) {
		Collection* collection = new Collection();
		Plane::PrimaryPlane->AddCollection(collection, true);

		// Read posistion
		read_float(file, buffer, 2);
		float x = *(float*)buffer, y = *(((float*)buffer)+1);
		collection->setPosition(sf::Vector2f(x, y));

		// Read stack count
		read_32(file, buffer, 1);
		uint32_t stack_count = *(uint32_t*)buffer;

		// Get stacks
		for(uint32_t stack_index = 0; stack_index < stack_count; stack_index++) {
			Stack* stack = new Stack();

			// Read posistion
			read_float(file, buffer, 2);
			float x = *(float*)buffer, y = *(((float*)buffer)+1);
			stack->setPosition(sf::Vector2f(x, y));

			collection->AddStack(stack);

			// Read block count
			read_32(file, buffer, 1);
			uint32_t block_count = *(((uint32_t*)buffer)+2);

			// Get blocks
			for(uint32_t block_index = 0; block_index < block_count; block_index++) {
				// Read name
				read_32(file, buffer, 1);
				uint32_t name_id = *(uint32_t*)buffer;

				// Read arg count
				uint8_t arg_count = file.get();

				Block* block = new Block(block_names[name_id]);
				stack->AddBlock(block);

				// Get args
				for(uint8_t arg_index = 0; arg_index < arg_count; arg_index++) {
					// Read arg size
					read_16(file, buffer, 1);
					uint16_t arg_size = *(uint16_t*)buffer;

					// Read arg data
					char* hbuffer = (char*)malloc(arg_size+1);
					hbuffer[arg_size] = 0;
					file.read(hbuffer, arg_size);

					// Add arg to block
					Argument* arg = new Argument();
					arg->SetData(hbuffer);
					block->AddArgument(arg);

					free(hbuffer);
				}
			}
		}
	}
}

std::string ProjectHandler::CurrentPath;

std::vector<std::string> ProjectHandler::Mods;
