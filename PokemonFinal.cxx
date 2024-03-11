#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>

#include <algorithm>
#include <string>
#include <cctype>

using std::cin;
using std::cout;

// Almacena cada stat del pokemon
struct Pokemon 
{
    std::string Name, Type1, Type2;
    int HP, Attack, Defense, SpAttack, SpDefense, Speed, Generation;
    bool Legendary;
};
// Contiene vector que almacena los datod de cada pokemon
struct pokemonTable
{
    std::vector <Pokemon> clasificacion;
    std::vector <Pokemon> pokemonData;
};
struct tpCombates
{
    std::string poke1, poke2, ganador;
    int valorP1, valorP2, diferencia;
} ;
// Inicializacion de Funciones
void muestraMenuOpciones(); // Imprime menu
void saveData(Pokemon &aux, std::string word, int counter); // Guarda contenido de string en tpPokemon
void printPokemonData(std::vector <Pokemon> v); // Imprime los Datos de cada pokemon
void readFile(pokemonTable &A, bool &isRead); // Lee el archivo csv
void mostrarPokemons(pokemonTable A); // Itera por cada indice en el vector pokemonData
void showLegendary(pokemonTable A); // Muestra los Pokemon Legendarios
void pokemonSin2T(pokemonTable A); // Muestra Pokemons sin segundo tipo
void printPowerful (pokemonTable A, Pokemon index); // Imprime los pokemon mas poderosos segun int data
void PokemonsMasPoder(pokemonTable A); // Itera buscando el int mas grande entre las mismas categorias de miembro
void imprimeDatosCombate(Pokemon poke1, Pokemon poke2);
int sumaTotalPuntos(Pokemon poke1);
void muestraMenuCombate();
void combate(Pokemon poke1, Pokemon poke2, tpCombates &datos_combate);
Pokemon seleccionaPokemon(pokemonTable &A);
std::string calcularVencedor(Pokemon poke1, Pokemon poke2);
void realizarCombate(Pokemon poke1, Pokemon poke2, tpCombates &showDown);
bool compare_strings_ignore_case(std::string str1, std::string str2);

int main(void)
{
    pokemonTable A; tpCombates showDown;
    bool fichero_OK = 0;
    int opcion;
    muestraMenuOpciones(); cin >> opcion;
    while (opcion != 0) // 0 = exit
    {
        switch (opcion)
        {
            case 1:
                readFile(A, fichero_OK);
                break;
            case 2: 
                if (fichero_OK == 1)
                {
                    mostrarPokemons(A);
                } else {
                    cout << "ERROR\nNo se puede ejecuter" 
                         <<  "si no se ha leido el fichero previamente (1)\n";
                }
                break;
            case 3:
                   if (fichero_OK == 1)
                {
                    pokemonSin2T(A);
                } else {
                    cout << "ERROR\nNo se puede ejecuter" 
                         <<  "si no se ha leido el fichero previamente (1)\n";
                } 
                break;
            case 4: 
                if (fichero_OK == 1)
                {
                    showLegendary(A);
                } else {
                    cout << "ERROR\nNo se puede ejecuter" 
                         <<  "si no se ha leido el fichero previamente (1)\n";
                }    
                break;
            case 5:
                if (fichero_OK == 1)
                {
                    PokemonsMasPoder(A);
                } else {
                    cout << "ERROR\nNo se puede ejecuter" 
                         <<  "si no se ha leido el fichero previamente (1)\n";
                }
                break;
            case 6: 
                if (fichero_OK == 1)
                {
                    Pokemon poke1 = seleccionaPokemon(A), poke2 = seleccionaPokemon(A);
                    combate(poke1, poke2, showDown);
                } else {
                    cout << "ERROR\nNo se puede ejecuter" 
                         <<  "si no se ha leido el fichero previamente (1)\n";
                }
                break;
            case 7:
                if (fichero_OK == 1)
                {   
                    std::fstream file ("fightRecord.csv", std::ios::out | std::ios::app);
                    if (file.is_open())
                    {
                        file << showDown.ganador << ','
                             << showDown.diferencia << ','
                             << showDown.poke1 << ','
                             << showDown.valorP1 << ','
                             << showDown.poke2 << ','
                             << showDown.valorP2 << '\n';
                        file.close();
                    } else {
                        cout << "Error al escribir esta scheisse\n";
                    }
                } else {
                    cout << "ERROR\nNo se puede ejecuter" 
                         <<  "si no se ha leido el fichero previamente (1)\n";
                }
                break;
            case 0: 
                return 0;
                break;
            default:
                cout << "ERROR opcion no valida.\n";
                break;
        }
        muestraMenuOpciones(); cin >> opcion;
    }
}
// Imprime el menu del juego
void muestraMenuOpciones()
{
    cout << "--------------------------------------------------------------------------\n"
         << "1. Leer Fichero de Pokemon\n"
         << "2. Mostrar todos los Pokemones\n"
         << "3. Listado de Pokemon que no tienen segundo tipo registrado\n"
         << "4. Listado de Pokemos Legendarios\n"
         << "5. Los Pokemon que tengan el valor mas alto en cada una de las categorias\n"
         << "6. Realizar combate\n"
         << "7. Guardar fichero de resultados de combate\n"
         << "0. Salir\n"
         << "--------------------------------------------------------------------------\n"
         << "Opcion: ";
}
/**
 * @brief saveData - Almacena los datos en el registro pokemon
 * 
 * @param aux Pokemon(registro auxiliar)
 * @param word string(palabra leida por cada linea)
 * @param counter Int(posicion de la plabra en la linea)
 */
void saveData(Pokemon &aux, std::string word, int counter)
{
    // Almacena (direccion en memeoria*) cada dato del registro con datos tipo int
    std::vector <int*> intData {&aux.HP, &aux.Attack, &aux.Defense,
                    &aux.SpAttack, &aux.SpDefense, &aux.Speed, &aux.Generation};
    // Datos* del registro tipo string de aux para luego ser modificados
    std::vector <std::string*> stringData {&aux.Name, &aux.Type1, &aux.Type2};
    if (counter >= 1 && counter <= 3) // Lugar de las columnas que son string
    {
        *stringData.at(counter-1) = word;
    }
    else if (counter >= 4 && counter <= 10) // Columnas donde son Int
    {
        *intData.at(counter-4) = std::stoi(word);
    } 
    else if (counter == 11) // Ultima columna tipo boolean
    {
        if (word == "True")
        {
            aux.Legendary = 1;
        } else {
            aux.Legendary = 0;
        }
    }
}

void printPokemonData(std::vector <Pokemon> v, int index)
{
    cout << "_____________________________________" << '\n';
    cout << "\t"<< index << "[] Pokemon: " << v.at(index).Name << std::endl;
    cout << "Type1: " << v.at(index).Type1 << std::endl;
    cout << "Type2: " << v.at(index).Type2 << std::endl;
    cout << "HP " << v.at(index).HP << "   "
         << "Attack " << v.at(index).Attack << "   "
         << "Defense " << v.at(index).Defense << std::endl;
    cout << "Sp. Attack " << v.at(index).SpAttack << "   "
         << "Sp. Defense " << v.at(index).SpDefense << std::endl
         << "Speed " << v.at(index).Speed << "   "
         << "Generation " << v.at(index).Generation << std::endl;
    cout << "Legendary: " << std::boolalpha << v.at(index).Legendary << std::endl;
}

void readFile(pokemonTable &A, bool &isRead) 
{
    std::string line, word;
    std::ifstream pokemon("pokemon.csv");
    if (pokemon.is_open())
    {
        Pokemon aux;
        std::getline(pokemon, line);
        while (pokemon)
        {
            std::getline(pokemon, line);
            std::stringstream s(line);
            int counter = 1; // Palabras por linea de texto
            while (getline(s, word, ','))
            {
                saveData(aux, word, counter);
                counter++;
            }
            A.pokemonData.push_back(aux); 
        }
        pokemon.close();
        cout << "Fichero leido correctamente\n";
        isRead = true;
    } else {
        cout << "ERROR al leer Pokemon.csv\n" << std::endl;
        isRead = false;
    }
}

void mostrarPokemons(pokemonTable A)
{
    for (int i = 0; i < A.pokemonData.size(); i++)
    {
        printPokemonData(A.pokemonData, i);
    }
}

void showLegendary(pokemonTable A)
{
    for (int i = 0; i < A.pokemonData.size(); i++)
    {
        if (((i+1)% 4) == 0) {cout << '\n';}
        if (A.pokemonData.at(i).Legendary = 1)
        {
            cout << '[' << i << "] " << A.pokemonData.at(i).Name << " ";
        }
    }
}

void pokemonSin2T(pokemonTable A)
{
    for (int i = A.pokemonData.size()- 1; i >= 0; i--)
    {
        if (((i+1)% 4) == 0) {cout << '\n';}
        if (A.pokemonData.at(i).Type2 == "")
        {
            cout << '[' << i << "] " << A.pokemonData.at(i).Name << " ";
        }
    }
}
// Almacena la ubicacion de los pokemones mas poderosos
void PokemonsMasPoder(pokemonTable A)
{
    Pokemon greatest, index;
    greatest.HP = 0; greatest.Attack = 0; greatest.SpAttack = 0; greatest.Defense = 0;
    greatest.SpDefense = 0; greatest.Speed = 0; greatest.Generation = 0;
    for (int i = A.pokemonData.size()- 1; i >= 0; i--)
    {
        if (A.pokemonData.at(i).HP > greatest.HP)
        {
            greatest.HP = A.pokemonData.at(i).HP;
            index.HP = i;
        }
        if (A.pokemonData.at(i).Attack > greatest.Attack)
        {
            greatest.Attack = A.pokemonData.at(i).Attack;
            index.Attack = i;
        }
        if (A.pokemonData.at(i).Defense > greatest.Defense)
        {
            greatest.Defense = A.pokemonData.at(i).Defense;
            index.Defense = i;
        }
        if (A.pokemonData.at(i).SpAttack > greatest.SpAttack)
        {
            greatest.SpAttack = A.pokemonData.at(i).SpAttack;
            index.SpAttack = i;
        }
        if (A.pokemonData.at(i).SpDefense > greatest.SpDefense)
        {
            greatest.SpDefense = A.pokemonData.at(i).SpDefense;
            index.SpDefense = i;
        }
        if (A.pokemonData.at(i).Speed > greatest.Speed)
        {
            greatest.Speed = A.pokemonData.at(i).Speed;
            index.Speed = i;
        }
        if (A.pokemonData.at(i).Generation > greatest.Generation)
        {
            greatest.Generation = A.pokemonData.at(i).Generation;
            index.Generation = i;
        }
    }
    printPowerful(A, index);
}

void printPowerful (pokemonTable A, Pokemon index)
{
    cout << "\tLOS MAS PODEROSOS\n"
         << A.pokemonData.at(index.HP).Name << "   HP  " << A.pokemonData.at(index.HP).HP << std::endl
         << A.pokemonData.at(index.Attack).Name << "   Attack  " << A.pokemonData.at(index.Attack).Attack << std::endl
         << A.pokemonData.at(index.Defense).Name << "   Defense  " << A.pokemonData.at(index.Defense).Defense << std::endl
         << A.pokemonData.at(index.SpAttack).Name << "   SpAttack  " << A.pokemonData.at(index.SpAttack).SpAttack << std::endl
         << A.pokemonData.at(index.SpDefense).Name << "   SpDefense  " << A.pokemonData.at(index.SpDefense).SpDefense << std::endl
         << A.pokemonData.at(index.Speed).Name << "   Speed  " << A.pokemonData.at(index.Speed).Speed << std::endl
         << A.pokemonData.at(index.Generation).Name << "   Generation  " << A.pokemonData.at(index.Generation).Generation << std::endl;
         
}

void muestraMenuCombate()
{
    cout << "--------------------------------------------------------------------------\n"
         << "1. Leer Fichero de Pokemon\n"
         << "2. Mostrar todos los Pokemones\n"
         << "3. Listado de Pokemon que no tienen segundo tipo registrado\n"
         << "0. Salir\n"
         << "--------------------------------------------------------------------------\n"
         << "Opcion: ";
}

Pokemon seleccionaPokemon(pokemonTable &A)
{
    srand(time(NULL));
    int index;
    bool a = 1;
    std::string name;
    getchar();
    do
    {
        cout << "\nNombre del pokemon: "; std::getline(cin, name); 
        for (int i = 0; i < A.pokemonData.size(); i++)
        {
            if ( compare_strings_ignore_case(name, A.pokemonData.at(i).Name) )
            {
                index = i;
                a = 0;
            } 
        }
        if (a)
        {
            cout << "El nombre escogido no corresponde, "
                 << "Porfavor escoge un pokemon que exista\n";
            cout << "E.g:\n";
            printPokemonData(A.pokemonData, (rand()%A.pokemonData.size()));
        }
    } while (a);
    return A.pokemonData.at(index);
}

void combate(Pokemon poke1, Pokemon poke2, tpCombates &datos_combate)
{
    imprimeDatosCombate(poke1, poke2);
    realizarCombate(poke1, poke2, datos_combate);
    if (calcularVencedor(poke1, poke2) == "EMPATE") 
    {
        cout << "El resultado es un EMPATE\n";
    } else {
        cout << "El ganador es " << datos_combate.ganador << std::endl;
    }
}
int sumaTotalPuntos(Pokemon poke1)
{
    return poke1.Attack+poke1.Defense+poke1.HP+poke1.SpAttack+poke1.SpDefense+poke1.Speed;
}

void imprimeDatosCombate(Pokemon poke1, Pokemon poke2)
{
    cout << "**********************************************************\n"
         << "Pokemon 1: " << poke1.Name << " Tipo: " << poke1.Type1 << " " << poke1.Type2 << '\n'
         << "Pokemon 2: " << poke2.Name << " Tipo: " << poke2.Type1 << " " << poke2.Type2 << '\n'
         << "**********************************************************\n"
         << "P1 ->" << "TOTAL " << sumaTotalPuntos(poke1)
         << "- HP " << poke1.HP << " - ATK " << poke1.Attack << " - DF " << poke1.Defense << " - SP. ATK " 
         << poke1.SpAttack << " - SP. DF " << poke1.SpDefense << " - SP " << poke1.Speed << '\n'
         << "P2 ->" << "TOTAL " << sumaTotalPuntos(poke2)
         << "- HP " << poke2.HP << " - ATK " << poke2.Attack << " - DF " << poke2.Defense << " - SP. ATK " 
         << poke2.SpAttack << " - SP. DF " << poke2.SpDefense << " - SP " << poke2.Speed << '\n';
}

void realizarCombate(Pokemon poke1, Pokemon poke2, tpCombates &showDown)
{
    showDown.ganador = calcularVencedor(poke1, poke2);
    showDown.diferencia = abs(sumaTotalPuntos(poke2) - sumaTotalPuntos(poke1));
    showDown.poke1 = poke1.Name; showDown.poke2 = poke2.Name;
    showDown.valorP1 = sumaTotalPuntos(poke1);
    showDown.valorP2 = sumaTotalPuntos(poke2);
}

std::string calcularVencedor(Pokemon poke1, Pokemon poke2)
{
    if (sumaTotalPuntos(poke1) > sumaTotalPuntos(poke2))
    {
        return poke1.Name;
    }
    else if (sumaTotalPuntos(poke1) < sumaTotalPuntos(poke2))
    {
        return poke2.Name;
    } else {
        return "EMPATE";
    }
}

bool compare_strings_ignore_case(std::string str1, std::string str2) 
{
    std::string str1_lower(str1);
    std::transform(str1_lower.begin(), str1_lower.end(), str1_lower.begin(), ::tolower);

    std::string str2_lower(str2);
    std::transform(str2_lower.begin(), str2_lower.end(), str2_lower.begin(), ::tolower);

    return std::equal(str1_lower.begin(), str1_lower.end(), str2_lower.begin(), str2_lower.end());

}