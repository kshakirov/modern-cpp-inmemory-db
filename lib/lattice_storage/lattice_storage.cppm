


// lattice_storage.cppm
#ifdef __clangd__
// Когда файл читает Emacs (clangd), мы притворяемся обычным кодом
#else
export module lattice_storage;
#endif

import std;

#ifdef __clangd__
// Для Emacs мы просто объявляем типы в глобальной области или пространстве имен
namespace lattice_storage {
#endif

// Твой чистый код, который увидят и GCC, и Emacs
export using NaiveTable = std::vector<std::vector<int>>;

export struct UserRow {
    int id;
    int age;
};

#ifdef __clangd__
} // namespace lattice_storage
using namespace lattice_storage; // Делаем доступным в main
#endif


export void hello_lattice() {
    std::cout << "LatticeDB: Модули успешно запущены!" << std::endl;
}
