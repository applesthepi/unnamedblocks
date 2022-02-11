#include "project.hpp"

void rhr::handler::project::load_project(const std::string& path)
{

}

void rhr::handler::project::save_project(const std::string& path)
{

}

/*using namespace cap::endianness;

inline void push(std::vector<char>& vec, u32 value)
{
	auto bytes = to_le_bytes(value);
	for (char i : bytes)
	{
		vec.push_back(i);
	}
}
inline void push(std::vector<char>& vec, f32 value)
{
	auto bytes = to_le_bytes(value);
	for (char i : bytes)
	{
		vec.push_back(i);
	}
}
inline void push(std::vector<char>& vec, i32 value)
{
	auto bytes = to_le_bytes(value);
	for (char i : bytes)
	{
		vec.push_back(i);
	}
}
inline void push(std::vector<char>& vec, u16 value)
{
	auto bytes = to_le_bytes(value);
	for (char i : bytes)
	{
		vec.push_back(i);
	}
}
inline void push(std::vector<char>& vec, std::string_view value)
{
	for (char i : value)
	{
		vec.push_back(i);
	}
}
inline void push(std::vector<char>& vec, char value) { vec.push_back(value); }*/
/*
// Reads count elements and converts them to the native endian. Stores the result in buffer
// Ensure buffer is atleast size * count bytes
inline void read_16(std::ifstream& file, char* buffer, u16 count) {
	file.read(buffer, count * 2);
	for(u16 i = 0; i < count; i++) {
		std::array<char, 2> bytes;
		bytes[0] = buffer[i * 2];
		bytes[1] = buffer[i * 2 + 1];
		buffer[i] = u16_from_le_bytes(bytes);
	}
}
// Reads count elements and converts them to the native endian. Stores the result in buffer
// Ensure buffer is atleast size * count bytes
inline void read_32(std::ifstream& file, char* buffer, u16 count) {
	file.read(buffer, count * 4);
	for(u16 i = 0; i < count; i++) {
		std::array<char, 4> bytes;
		bytes[0] = buffer[i * 2];
		bytes[1] = buffer[i * 2 + 1];
		bytes[2] = buffer[i * 2 + 2];
		bytes[3] = buffer[i * 2 + 3];
		buffer[i] = u32_from_le_bytes(bytes);
	}
}
const auto& read_float = read_32;
const auto& read_f32 = read_32;

// Layout
// Indent = Can repeat
// FIXME: Endianness
//
// (u32) mod_count
//   (u8) name_size (char[]) name
// (u32) name_count
//   (u8) name_size (char[]) name
//
// Stack:
// (u32) collection_count
//   (f32) collection_x collection_y (u32) stack_count
//     (f32) stack_x stack_y (u32)block_count
//       (u32) name_id (u8) arg_count
//         (u16) arg_size (char[]) arg
void ProjectHandler::SaveProject(const std::string& path) {
	std::unordered_map<std::string, u32> mod_ids;
	std::unordered_map<const char*, u32> block_ids;
	std::vector<std::string> mod_names;
	std::vector<std::string> block_names;
	u32 next_id = 0;
	const std::vector<Collection*>& collections = Plane::PrimaryPlane->GetCollections();

	//  Rough estimates
	std::vector<char> header;
	std::vector<char> body;
	header.reserve(1000 * collections.size());
	body.reserve(10000 * collections.size());

	push(body, static_cast<u32>(collections.size()));
	// Collections
	for(Collection* i : collections) {
		const std::vector<Stack*>& stacks = i->GetStacks();
		push(body, i->getPosition().x);
		push(body, i->getPosition().y);
		push(body, static_cast<u32>(stacks.size()));
		for(Stack* stack : stacks) {
			sf::Vector2f position = stack->getPosition();
			const std::vector<Block*>& blocks = stack->GetBlocks();

			push(body, position.x);
			push(body, position.y);
			push(body, static_cast<u32>(blocks.size()));

			for(Block* block : blocks) {
				const char* block_name = block->GetModBlock()->GetUnlocalizedName();
				const std::string& mod_name = BlockRegistry::GetRegistry().GetBlockMod(block->GetModBlock());
				const std::vector<Argument*>& arguments = block->GetArguments();
				u32 id;

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

				push(body, id);
				// Put placeholder argument count
				push(body, 0);
				// Save the placeholder position for later
				const u64 pos = body.size();
				u8 argument_count = 0;
				for(Argument* argument : arguments) {
					if(!argument->HasData()) continue;
					argument_count++;
					std::string data = argument->GetData();
					data.insert(data.begin(), argument->GetMode() == BlockArgumentVariableMode::RAW ? '0' : '1');
					// Check size
					UB_ASSERT(data.size() < 65535);

					push(body, (u16)data.size());
					push(body, data);
				}
				body[pos] = argument_count;
			}
		}
	}
	push(header, (u32)mod_ids.size());
	for(std::string& i : mod_names) {
		UB_ASSERT(i.size() < 255);
		push(header, (u8)i.size());
		push(header, i);
	}
	push(header, (u32)block_ids.size());
	for(std::string& i : block_names) {
		UB_ASSERT(i.size() < 255);
		push(header, (u8)i.size());
		push(header, i);
	}
	std::ofstream file(path, std::ofstream::trunc | std::ofstream::out | std::ofstream::binary);
	file.write(header.data(), static_cast<i64>(header.size()));
	file.write(body.data(), static_cast<i64>(body.size()));
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
		cap::logger::Error("unable to open file \"" + path + "\"");
		return;
	}
	// Reset plane
	sf::View* primaryView = Plane::PrimaryPlane->GetView();
	primaryView->setCenter(primaryView->getSize() / 2.0f);
	Plane::PrimaryPlane->DeleteContents();

	// Read mod header
	read_32(file, buffer, 1);

	u32 mod_name_count = *(u32*)buffer;
	std::string* mod_names = new std::string[mod_name_count];
	for(u32 mod_name_index = 0; mod_name_index < mod_name_count; mod_name_index++) {
		u8 name_size = file.get();
		mod_names[mod_name_index].reserve(name_size);
		file.read(buffer, name_size);
		buffer[name_size] = 0;
		mod_names[mod_name_index] += buffer;
	}

	// Read block header
	read_32(file, buffer, 1);

	u32 block_name_count = *(u32*)buffer;
	std::string* block_names = new std::string[block_name_count];

	for(u32 block_name_index = 0; block_name_index < block_name_count; block_name_index++) {
		u8 name_size = file.get();
		block_names[block_name_index].reserve(name_size);
		file.read(buffer, name_size);
		buffer[name_size] = 0;
		block_names[block_name_index] += buffer;
	}
	// Read collection count
	read_32(file, buffer, 1);
	u32 collection_count = *(u32*)buffer;

	// Get collections
	for(u32 collection_index = 0; collection_index < collection_count; collection_index++) {
		Collection* collection = new Collection();
		Plane::PrimaryPlane->AddCollection(collection, true);

		// Read posistion
		read_float(file, buffer, 2);
		f32 x = *(f32*)buffer, y = *(((f32*)buffer)+1);
		collection->setPosition(sf::Vector2f(x, y));

		// Read stack count
		read_32(file, buffer, 1);
		u32 stack_count = *(u32*)buffer;

		// Get stacks
		for(u32 stack_index = 0; stack_index < stack_count; stack_index++) {
			Stack* stack = new Stack();

			// Read posistion
			read_float(file, buffer, 2);
			f32 x = *(f32*)buffer, y = *(((f32*)buffer)+1);
			stack->setPosition(sf::Vector2f(x, y));

			collection->AddStack(stack);

			// Read block count
			read_32(file, buffer, 1);
			u32 block_count = *(((u32*)buffer)+2);

			// Get blocks
			for(u32 block_index = 0; block_index < block_count; block_index++) {
				// Read name
				read_32(file, buffer, 1);
				u32 name_id = *(u32*)buffer;

				// Read arg count
				u8 arg_count = file.get();

				Block* block = new Block(block_names[name_id]);
				stack->AddBlock(block);

				// Get args
				for(u8 arg_index = 0; arg_index < block->GetArguments().size(); arg_index++) {

					if (!block->GetArguments()[arg_index]->HasData())
						continue;

					// Read arg size
					read_16(file, buffer, 1);
					u16 arg_size = *(u16*)buffer;

					// Read arg data
					char* hbuffer = (char*)malloc(arg_size+1);
					hbuffer[arg_size] = 0;
					file.read(hbuffer, arg_size);

					// Update arg data

					block->GetArguments()[arg_index]->SetMode(hbuffer[0] == '0' ? BlockArgumentVariableMode::RAW : BlockArgumentVariableMode::VAR);
					block->GetArguments()[arg_index]->SetData(hbuffer + 1);
					block->GetArguments()[arg_index]->UpdateData();

					free(hbuffer);
				}
			}
		}
	}
}
*/
std::string rhr::handler::project::current_path;

std::vector<std::string> rhr::handler::project::mods;
