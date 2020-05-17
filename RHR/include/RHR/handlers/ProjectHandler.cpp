#include "ProjectHandler.h"
#include <unordered_map>
#include <fstream>
#include <string_view>

void ProjectHandler::Initialize()
{
	CurrentPath = std::string();
}

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
inline void push(std::vector<char> &vec, u32 value) {
	for(char i : value.bytes) {
		vec.push_back(i);
	}
}
inline void push(std::vector<char> &vec, i32 value) {
	for(char i : value.bytes) {
		vec.push_back(i);
	}
}
inline void push(std::vector<char> &vec, u16 value) {
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
// Layout
// Indent = Can repeat
// FIXME: Endianness
//
// (uint32_t) name_count
//   (uint8_t) name_size (char[]) name
//
// Stack:
// (uint32_t) stack_count
//   (int32_t) x y (uint32_t)block_count
//     (uint32_t) name_id (uint8_t) arg_count
//       (uint16_t) arg_size (char[]) arg
void ProjectHandler::SaveProject(std::string& path, Plane* plane) {
	std::unordered_map<std::string, uint32_t> ids;
	std::vector<std::string> names;
	uint32_t next_id = 0;
	uint32_t stack_count = plane->GetStackCount();

	//  Rough estimates
	std::vector<char> header;
	std::vector<char> body;
	header.reserve(100 * stack_count);
	body.reserve(1000 * stack_count);

	push(body, u32 {stack_count});
	for(uint32_t stack_index = 0; stack_index < stack_count; stack_index++) {
		Stack* stack = plane->GetStack(stack_index);
		auto position = stack->GetSetPosition();
		uint32_t block_count = stack->GetBlockCount();

		push(body, i32 {position.x});
		push(body, i32 {position.y});
		push(body, u32 {block_count});

		for(uint32_t block_index = 0; block_index < block_count; block_index++) {
			Block* block = stack->GetBlock(block_index);
			std::string name = block->GetUnlocalizedName();
			uint8_t arg_count = block->GetArgumentCount();
			for(uint32_t i = 0; i < block->GetArgumentCount(); i++) {
				if(!block->GetArgument(i)->HasData()) arg_count--;
			}
			uint32_t id;

			// Set name id if not already a thing
			if (ids.contains(name)) {
				id = ids[name];
			}
			else {
				id = next_id++;
				ids[name] = id;
				names.push_back(name);
			}

			push(body, u32 {id});
			push(body, u8 {arg_count});
			for(uint32_t i = 0; i < block->GetArgumentCount(); i++) {
				Argument* arg = block->GetArgument(i);
				if(!arg->HasData()) continue;
				std::string* data = arg->GetData();
				// TODO: Check if size is too big
				push(body, u16 {(uint16_t)data->size()});
				push(body, *data);
			}
		}
	}
	push(header, u32 {(uint32_t)ids.size()});
	for(std::string& i : names) {
		push(header, u8 {(uint8_t)i.size()});
		push(header, i);
	}
	std::ofstream file(path, std::ofstream::trunc | std::ofstream::out | std::ofstream::binary);
	file.write(header.data(), static_cast<int64_t>(header.size()));
	file.write(body.data(), static_cast<int64_t>(body.size()));
	file.close();
}

void ProjectHandler::LoadProject(std::string& path, Plane* plane, BlockRegistry* registry)
{
	char buffer[257];
	std::ifstream file(path, std::ifstream::in | std::ifstream::binary);

	if (!file.is_open())
	{
		Logger::Error("unable to open file \"" + path + "\"");
		return;
	}

	plane->SetInnerPosition(sf::Vector2i(0, 0));
	plane->DeleteAllBlocks();

	file.read(buffer, sizeof(uint32_t));

	uint32_t name_count = *(uint32_t*)buffer;
	std::string* names = new std::string[name_count];
	// Read header
	for(uint32_t name_index = 0; name_index < name_count; name_index++) {
		uint8_t name_size = file.get();
		names[name_index].reserve(name_size);
		file.read(buffer, name_size);
		buffer[name_size] = 0;
		names[name_index] += buffer;
	}
	file.read(buffer, sizeof(uint32_t));
	uint32_t stack_count = *(uint32_t*)buffer;
	// Get stacks
	for(uint32_t stack_index = 0; stack_index < stack_count; stack_index++) {
		file.read(buffer, sizeof(uint32_t) * 3);
		int32_t x = *(int32_t*)buffer, y = *(((int32_t*)buffer)+1);

		Stack* stack = new Stack(sf::Vector2i(x, y), registry);
		plane->AddStack(stack);
		uint32_t block_count = *(((uint32_t*)buffer)+2);

		// Get blocks
		for(uint32_t block_index = 0; block_index < block_count; block_index++) {
			file.read(buffer, sizeof(uint32_t));
			uint32_t name_id = *(uint32_t*)buffer;
			uint8_t arg_count = file.get();

			Block* block = new Block(names[name_id], registry, stack->GetFunctionUpdate(), stack->GetFunctionSelect());
			std::vector<std::string> args;
			// Get args
			for(uint8_t arg_index = 0; arg_index < arg_count; arg_index++) {
				file.read(buffer, sizeof(uint16_t));
				uint16_t arg_size = *(uint16_t*)buffer;
				char* hbuffer = (char*)malloc(arg_size+1);
				hbuffer[arg_size] = 0;
				file.read(hbuffer, arg_size);
				args.emplace_back(hbuffer);
				free(hbuffer);
			}
			block->SetArgData(args);
			stack->AddBlock(block);
		}
	}
	plane->ReloadVanity();
	plane->FrameUpdate(registry, true);
}

std::string ProjectHandler::CurrentPath;

std::vector<std::string> ProjectHandler::Mods;
