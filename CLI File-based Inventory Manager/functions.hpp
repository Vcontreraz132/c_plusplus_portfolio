/*---------------------------------------------------------------------------------*/
                                /*Item Class*/
class Item {
    int id;
    std::string name;
    double price;
    int quantity;

public:
    void init(int id, std::string name, double price, int quantity) {
        this->id = id;
        this->name = name;
        this->price = price;
        this->quantity = quantity;
    }
    void display() const {
        std::cout << "ID: " << id << ", Name: " << name
                  << ", Price: " << price << ", Quantity: " << quantity << std::endl;
    }
    // Getters for item properties
    int getId() const { return id; }
    std::string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    // Setters for item properties
    void setId(int newId) { id = newId; }
    void setName(const std::string& newName) { name = newName; }
    void setPrice(double newPrice) { price = newPrice; }
    void setQuantity(int newQuantity) { quantity = newQuantity; }   
};
/*---------------------------------------------------------------------------------*/

int createSave(std::string save_directory);

bool scanSaveDirectory(std::string save_directory, std::string target_extension);

void printMenu();

std::vector<Item> loadSave(std::string save_directory, std::string save_file_name);

void showAllItems(const std::vector<Item>& allItems);