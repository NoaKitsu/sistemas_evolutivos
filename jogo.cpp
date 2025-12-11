#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <ctime>

using namespace std;

/*
Elemento:
1:agua
2:fogo
3:terra
4:ar
5:luz
6:escuridao

Interacoes especiais(multiplicador de dano em casos especiais, senão é 1x):
1 -> 2 = 2x
2 -> 1 = 0.8x
2 -> 3 = 2x
3 -> 2 = 0.8x
3 -> 4 = 2x
4 -> 3 = 0.8x
4 -> 1 = 2x
1 -> 4 = 0.8x
5 -> 6 = 1.5x
6 -> 5 = 1.5x

*/

struct Personagem{
    float vida;
    int dano_fisico;
    int dano_magico;
    int resistencia_fisica;
    int resistencia_magica;
    int velocidade;
    int elemento;

    float fitness;
};
//Obs: possuem mesma estrutura que o personagem no momento, mas creio na possibilidade de evoluir o código futuramente
struct Inimigo{
    float vida;
    int dano_fisico;
    int dano_magico;
    int resistencia_fisica;
    int resistencia_magica;
    int velocidade;
    int elemento;

    float fitness;
};

struct ResultadoBatalha{
    bool venceu;
    float vidaRestantePersonagem;
    float vidaRestanteInimigo;
    int rodadas;
};

// Gera números aleatórios entre um min e um max
float random_number(int min, int max) {
    static std::mt19937 gen(time(nullptr));
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

// Multiplicador de dano relativo ao elemento
float multiplicadorElemento(float atacante, float defensor){
    if (atacante == 1 && defensor == 2) return 2.0f;
    if (atacante == 2 && defensor == 1) return 0.8f;
    if (atacante == 2 && defensor == 3) return 2.0f;
    if (atacante == 3 && defensor == 2) return 0.8f;
    if (atacante == 3 && defensor == 4) return 2.0f;
    if (atacante == 4 && defensor == 3) return 0.8f;
    if (atacante == 4 && defensor == 1) return 2.0f;
    if (atacante == 1 && defensor == 4) return 0.8f;
    if (atacante == 5 && defensor == 6) return 1.5f;
    if (atacante == 6 && defensor == 5) return 1.5f;

    return 1.0f;
}

Personagem gerarPersonagem(){
    Personagem personagem;
    personagem.vida = random_number(100, 200); //vida
    personagem.dano_fisico = random_number(10, 40); //dano fisico
    personagem.dano_magico = random_number(10, 40); //dano magico
    personagem.resistencia_fisica = random_number(1, 70); //resistencia fisica (0-70%)
    personagem.resistencia_magica = random_number(1, 50); //resistencia magica (0-50%)
    personagem.velocidade = random_number(10, 50); //velocidade
    personagem.elemento = random_number(1,6); //elemento
    personagem.fitness = personagem.vida * 0.35f +
        personagem.dano_fisico * 0.2f +
        personagem.dano_magico * 0.15f +
        personagem.resistencia_fisica * 0.1f +
        personagem.resistencia_magica * 0.05f +
        personagem.velocidade * 0.15f;
    return personagem;
}

Inimigo gerarInimigo(){
    Inimigo inimigo;
    inimigo.vida = random_number(150, 250); //vida
    inimigo.dano_fisico = random_number(10, 40); //dano fisico
    inimigo.dano_magico = random_number(10, 40); //dano magico
    inimigo.resistencia_fisica = random_number(1, 70); //resistencia fisica (0-70%)
    inimigo.resistencia_magica = random_number(1, 50); //resistencia magica (0-50%)
    inimigo.velocidade = random_number(10, 50); //velocidade
    inimigo.elemento = random_number(1,6); //elemento
    inimigo.fitness = inimigo.vida * 0.35f +
        inimigo.dano_fisico * 0.2f +
        inimigo.dano_magico * 0.15f +
        inimigo.resistencia_fisica * 0.1f +
        inimigo.resistencia_magica * 0.05f +
        inimigo.velocidade * 0.15f;
    return inimigo;
}

// Calcula o fitness do inimigo(Não implementado/planejado mudanças para ele já que o foco são os personagens)

void mostrarStatusPersonagem(const Personagem& personagem){
    cout << "Vida: " << personagem.vida << " | ";
    cout << "Dano F: " << personagem.dano_fisico << " | ";
    cout << "Dano M: " << personagem.dano_magico << " | ";
    cout << "Resistencia F: " << personagem.resistencia_fisica << "%  | ";
    cout << "Resistencia M: " << personagem.resistencia_magica << "%  | ";
    cout << "Velocidade: " << personagem.velocidade << " | ";
    cout << "Elemento: " << personagem.elemento << " | ";
    cout << "Fitness: " << personagem.fitness << "\n";
}

void mostrarStatusInimigo(const Inimigo& inimigo){
    cout << "Vida: " << inimigo.vida << " | ";
    cout << "Dano Fisico: " << inimigo.dano_fisico << " | ";
    cout << "Dano Magico: " << inimigo.dano_magico << " | ";
    cout << "Resistencia Fisica: " << inimigo.resistencia_fisica << "% | ";
    cout << "Resistencia Magica: " << inimigo.resistencia_magica << "% | ";
    cout << "Velocidade: " << inimigo.velocidade << " | ";
    cout << "Elemento: " << inimigo.elemento << " | ";
    cout << "Fitness: " << inimigo.fitness << "\n";
}

// Mostra a batalha entre o personagem x e o inimigo
void mostrarBatalha(const Personagem& personagem, const Inimigo& inimigo) {
    cout << "=============================\n";
    cout << "      STATUS DA BATALHA      \n";
    cout << "=============================\n";

    cout << ">>> PERSONAGEM <<<\n";
    mostrarStatusPersonagem(personagem);
    cout << ">>> INIMIGO <<<\n";
    mostrarStatusInimigo(inimigo);

    cout << "============================\n";
}

ResultadoBatalha lutar(const Personagem& personagem, const Inimigo& inimigo){
    float vidaP = personagem.vida;
    float vidaI = inimigo.vida;

    int rodadas = 0;

    // decide quem inicia o combate
    bool turnoPersonagem = personagem.velocidade >= inimigo.velocidade;

    while (vidaP > 0 && vidaI > 0 && rodadas < 1000) {
        rodadas++;

        if (turnoPersonagem) {
            // personagem ataca
            float mult = multiplicadorElemento(personagem.elemento, inimigo.elemento);

            if(rodadas%3 == 0){
                // dano mágico reduzido pela resistência em %
                float danoMag = personagem.dano_magico * (1.0f - (inimigo.resistencia_magica / 100.0f));
                if (danoMag < 1) danoMag = 1;
                vidaI -= danoMag * mult;
            } else{
                // dano físico reduzido pela resistência em %
                float danoFis = personagem.dano_fisico * (1.0f - (inimigo.resistencia_fisica / 100.0f));
                if (danoFis < 1) danoFis = 1;
                vidaI -= danoFis * mult;
            }
        }
        else {
            // inimigo ataca
            float mult = multiplicadorElemento(inimigo.elemento, personagem.elemento);

            if(rodadas%3 == 0){
                // dano mágico reduzido pela resistência em %
                float danoMag = inimigo.dano_magico * (1.0f - (personagem.resistencia_magica / 100.0f));
                if (danoMag < 1) danoMag = 1;
                vidaP -= danoMag * mult;
            } else{
                // dano físico reduzido pela resistência em %
                float danoFis = inimigo.dano_fisico * (1.0f - (personagem.resistencia_fisica / 100.0f));
                if (danoFis < 1) danoFis = 1;
                vidaP -= danoFis * mult;
            }
        }

        // alterna o turno
        turnoPersonagem = !turnoPersonagem;
    }

    ResultadoBatalha r;
    r.venceu = vidaI <= 0;
    r.vidaRestantePersonagem = max(0.0f, vidaP);
    r.vidaRestanteInimigo = max(0.0f, vidaI);
    r.rodadas = rodadas;

    return r;
}

// guarda os dados da batalha
vector<ResultadoBatalha> batalharGeracao(vector<Personagem> populacao, Inimigo inimigo) {
    vector<ResultadoBatalha> resultados;
    resultados.reserve(populacao.size());

    for (auto& p : populacao) {
        resultados.push_back(lutar(p, inimigo));
    }

    return resultados;
}

class EvolutionSystem{
    public:
        int tamanho_populacao = 20;
        float taxa_mutacao = 20;
        float percentual_elite = 0.50f;

        vector<Personagem> populacao;
        Inimigo inimigo;

        EvolutionSystem(){
            gerarPopulacaoInicial();
            gerarNovoInimigo();
        }
        
        // Cria a população inicial
        void gerarPopulacaoInicial(){
            populacao.clear();
            populacao.reserve(tamanho_populacao);
            for (int i = 0; i < tamanho_populacao; i++) {
                populacao.push_back(gerarPersonagem());
            }
            sort(populacao.begin(), populacao.end(),
                [](const Personagem& a, const Personagem& b){
                    return a.fitness > b.fitness;
                });
        }

        // Cria o inimigo inicial
        void gerarNovoInimigo(){
            inimigo = gerarInimigo();
        }

        // Recalcula o fitness com base no resultado da batalha
        void avaliarFitnessPosBatalha(int i, bool venceu, float vidaRestantePersonagem, float vidaRestanteInimigo, int rodadas){
            float anterior = populacao[i].fitness;
            if(venceu){
                populacao[i].fitness = populacao[i].fitness + vidaRestantePersonagem/10 + (100.0f/(float)rodadas);
            } else{
                populacao[i].fitness = populacao[i].fitness - vidaRestanteInimigo/((float)rodadas/2) + (16.0f/((float)rodadas*100.0f));
            }
            
            cout << "Fitness de " << i << " | Anterior = " << anterior << " | Posterior = " << populacao[i].fitness << "\n";
        }

        // Seleção dos melhores
        vector<Personagem> selecionarMelhores(){
            sort(populacao.begin(), populacao.end(),
                [](const Personagem& a, const Personagem& b){
                    return a.fitness > b.fitness;
                });

            int eliteCount = tamanho_populacao * percentual_elite;
            return vector<Personagem>(populacao.begin(), populacao.begin() + eliteCount);
        }

        // Cruzamento
        Personagem cruzar(Personagem& pai, Personagem& mae){
            Personagem filho;
                filho.vida = random_number(1,10) < 5 ? pai.vida : mae.vida;
                filho.dano_fisico = random_number(1,10) < 5 ? pai.dano_fisico : mae.dano_fisico;
                filho.dano_magico = random_number(1,10) < 5 ? pai.dano_magico : mae.dano_magico;
                filho.resistencia_fisica = random_number(1,10) < 5 ? pai.resistencia_fisica : mae.resistencia_fisica;
                filho.resistencia_magica = random_number(1,10) < 5 ? pai.resistencia_magica : mae.resistencia_magica;
                filho.velocidade = random_number(1,10) < 5 ? pai.velocidade : mae.velocidade;
                if(random_number(1,100) <= 25){
                    filho.elemento = random_number(1,6);
                } else{
                    filho.elemento = random_number(1,10) < 5 ? pai.elemento : mae.elemento;
                }
                
                filho.fitness = filho.vida * 0.35f +
                    filho.dano_fisico * 0.2f +
                    filho.dano_magico * 0.15f +
                    filho.resistencia_fisica * 0.1f +
                    filho.resistencia_magica * 0.05f +
                    filho.velocidade * 0.15f;

            return filho;
        }

        // Mutação
        void mutar(Personagem& personagem){
            if (random_number(1,100) < taxa_mutacao) personagem.vida += random_number(-5,15);
            if (random_number(1,100) < taxa_mutacao) personagem.dano_fisico += random_number(-2,5);
            if (random_number(1,100) < taxa_mutacao) personagem.dano_magico += random_number(-2,5);
            if (random_number(1,100) < taxa_mutacao) personagem.resistencia_fisica += random_number(-5,10);
            if (random_number(1,100) < taxa_mutacao) personagem.resistencia_magica += random_number(-5,10);
            if (random_number(1,100) < taxa_mutacao) personagem.velocidade += random_number(-5,15);
            if (random_number(1,50) < taxa_mutacao) personagem.elemento = random_number(1,6);



            // limitacao dos valores
            if (personagem.vida < 100) personagem.vida = 100;
            if (personagem.vida > 200) personagem.vida = 200;
            if (personagem.dano_fisico < 10) personagem.dano_fisico = 10;
            if (personagem.dano_fisico > 40) personagem.dano_fisico = 40;
            if (personagem.dano_magico < 10) personagem.dano_magico = 10;
            if (personagem.dano_magico > 40) personagem.dano_magico = 40;
            if (personagem.resistencia_fisica < 1) personagem.resistencia_fisica = 1;
            if (personagem.resistencia_fisica > 70) personagem.resistencia_fisica = 70;
            if (personagem.resistencia_magica < 1) personagem.resistencia_magica = 1;
            if (personagem.resistencia_magica > 50) personagem.resistencia_magica = 50;
            if (personagem.velocidade < 10) personagem.velocidade = 10;
            if (personagem.velocidade > 50) personagem.velocidade = 50;

            personagem.fitness = personagem.vida * 0.35f +
                personagem.dano_fisico * 0.2f +
                personagem.dano_magico * 0.15f +
                personagem.resistencia_fisica * 0.1f +
                personagem.resistencia_magica * 0.05f +
                personagem.velocidade * 0.15f;
        }

        // Gera uma nova geração com base na anterior
        void gerarNovaGeracao(bool predacao){
            auto elite = selecionarMelhores();
            vector<Personagem> novaPop;

            // mantém elite
            for (auto& e : elite) novaPop.push_back(e);

            //reseta o fitness da elite
            for (int i = 0; i < novaPop.size(); i++){
                novaPop[i].fitness = novaPop[i].vida * 0.35f +
                    novaPop[i].dano_fisico * 0.2f +
                    novaPop[i].dano_magico * 0.15f +
                    novaPop[i].resistencia_fisica * 0.1f +
                    novaPop[i].resistencia_magica * 0.05f +
                    novaPop[i].velocidade * 0.15f;
            }

            //predacao de elites
            if(predacao){
                int j = random_number(0,2);
                while (j > 0)
                {
                    novaPop.erase(novaPop.begin()+random_number(1, novaPop.size()));
                    j--;
                }
            }

            // crossover + mutação
            int i = 5;
            while (novaPop.size() < (unsigned)tamanho_populacao){
                if(i > 0){
                    novaPop.push_back(gerarPersonagem());
                }else{
                    Personagem& pai = elite[rand() % elite.size()];
                    Personagem& mae = elite[rand() % elite.size()];

                    Personagem filho = cruzar(pai, mae);
                    if(random_number(1,10) < 8) mutar(filho);
                    novaPop.push_back(filho);
                }
                i--;
            }
            populacao = novaPop;
        }
};

int main() {
    EvolutionSystem evo;
    int opcao;
    int geracao = 0;
    bool predacao = false;
    cout << "Uma geracao e um inimigo inicial foi formado!\n";
    while(true){
        cout << "\n===== MENU =====\n";
        cout << "1 - Mostrar geracao atual: " << geracao << "\n";
        cout << "2 - Mostrar inimigo atual\n";
        cout << "3 - Gerar nova geracao inicial\n";
        cout << "4 - Gerar novo inimigo\n";
        cout << "5 - Rodar evolucao(batalha e gera uma nova geracao)\n";
        cout << "6 - Predacao Habilitada na evolucao?(0 - nao, 1 - sim): " << predacao <<"\n";
        cout << "0 - Sair\n";
        cout << "Escolha: ";
        cin >> opcao;

        if(opcao == 1){
            for(int i = 0; i < evo.tamanho_populacao; i++){
                cout << "Personagem " << i << ":\n";
                mostrarStatusPersonagem(evo.populacao[i]);
            }
        }else if(opcao == 2){
            mostrarStatusInimigo(evo.inimigo);
        }else if(opcao == 3){
            evo.gerarPopulacaoInicial();
            geracao = 0;

            cout << "Nova geracao inicial formada:\n";

            for(int i = 0; i < evo.tamanho_populacao; i++){
                cout << "Personagem " << i << ":\n";
                mostrarStatusPersonagem(evo.populacao[i]);
            }
            
        }else if(opcao == 4){
            evo.gerarNovoInimigo();

            cout << "Novo inimigo gerado:\n";

            mostrarStatusInimigo(evo.inimigo);
        }else if(opcao == 5){
            cout << "\nEvoluindo geracao de personagens\n";
            
            auto resultados = batalharGeracao(evo.populacao, evo.inimigo);

            cout << "\nResultados da geracao " << geracao << "\n";

            for (int i = 0; (unsigned)i < resultados.size(); i++) {
                cout << "Personagem " << i << ":\n";
                mostrarBatalha(evo.populacao[i], evo.inimigo);
                cout << "Resultado " << i << "| ";
                cout << (resultados[i].venceu ? "VENCEU" : "PERDEU");
                cout << " | Vida restante personagem: " << resultados[i].vidaRestantePersonagem;
                cout << " | Vida restante inimigo: " << resultados[i].vidaRestanteInimigo;
                cout << " | Rodadas: " << resultados[i].rodadas << "\n";
                evo.avaliarFitnessPosBatalha(i, resultados[i].venceu, resultados[i].vidaRestantePersonagem, resultados[i].vidaRestanteInimigo, resultados[i].rodadas);
                cin.get();
            }

            evo.gerarNovaGeracao(predacao);

            cout << "Geracao " << geracao << " concluida!\n";
            geracao++;
        } else if(opcao == 6){
            predacao = !predacao;
        } else if(opcao == 0){
            cout << "Encerrando o programa\n";
            break;
        } else{
            cout << "Opcao invalida!\n";
        }
    }

    return 0;
}
