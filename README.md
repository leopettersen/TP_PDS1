# Trabalho Prático de Programação e Desenvolvimento de Software 1

Sistema de gerenciamento de estações meteorológicas em C, com cadastro,
consulta, edição, remoção, análise estatística e persistência em CSV.

## Descrição

Estações meteorológicas coletam continuamente dados ambientais por meio de
sensores (por exemplo, temperatura do ar, umidade relativa, pressão
atmosférica e velocidade do vento). Essas leituras precisam ser armazenadas,
processadas e analisadas para gerar informações úteis como média, variância
e desvio-padrão das medições, além de permitir a detecção de leituras
anômalas que possam indicar falhas de sensor ou eventos climáticos
relevantes.

Este programa oferece um **menu interativo no terminal** para realizar todas
essas operações em até 100 estações simultaneamente, com persistência em
arquivo CSV.

## Integrantes do Grupo

- Leonardo Federici Pettersen
- João Victor Resende

## Professor Orientador

- Pedro Henrique Silva Souza Barros

## Estrutura dos arquivos

```
TP_PDS1/
├── README.md
├── .gitignore
├── src/
│   ├── main.c              # Programa principal: menu e despacho das opções
│   └── func/
│       ├── funcoes.h       # Declarações de structs e protótipos das funções
│       └── funcoes.c       # Implementações das funções
└── estacoes.csv            # Arquivo CSV gerado/lido pelo programa (runtime)
```

## Como compilar e executar

### Pré-requisitos

- Compilador C compatível com C99 (GCC recomendado).
  - **Windows:** MinGW-w64 (ex.: via MSYS2 em `C:\msys64\mingw64\bin`).
  - **Linux/macOS:** GCC ou Clang.

### Compilação

A partir da **raiz do projeto** (`TP_PDS1/`), rode:

```bash
gcc src/main.c src/func/funcoes.c -o main -lm
```

A flag `-lm` faz o link com a biblioteca matemática (necessária por causa
de `sqrt` e `fabs` em `<math.h>`). Em Linux ela é obrigatória; no MinGW
geralmente não é, mas manter `-lm` é seguro em qualquer ambiente.

Para ver os avisos do compilador:

```bash
gcc src/main.c src/func/funcoes.c -o main -lm -Wall
```

No Windows com PowerShell, se o GCC do MinGW não estiver no PATH:

```powershell
$env:PATH = "C:\msys64\mingw64\bin;" + $env:PATH
gcc src/main.c src/func/funcoes.c -o main.exe -lm -Wall
```

### Execução

Linux/macOS:

```bash
./main
```

Windows:

```powershell
.\main.exe
```

## Funcionalidades (menu principal)

| Opção | Ação                | Descrição                                                                                       |
| ----- | ------------------- | ----------------------------------------------------------------------------------------------- |
| 1     | Adicionar estação   | Cadastra uma estação validando data, intervalo de N (1–9999) e unicidade do ID. Aloca `leituras` com `malloc` e calcula estatísticas. |
| 2     | Listar estações     | Exibe todas as estações com dados completos e estatísticas. Data em `dd/mm/aaaa`.               |
| 3     | Buscar por operador | Lista as estações de um operador específico (comparação via `strcmp`).                          |
| 4     | Editar estação      | Submenu para alterar nome, operador, sensor ou data de uma estação localizada por ID.           |
| 5     | Remover estação     | Remove uma estação pelo ID, fazendo `free` no vetor `leituras` correspondente.                  |
| 6     | Detectar anomalias  | Lista leituras fora do intervalo `[média − 2σ, média + 2σ]`, usando `fabs`.                     |
| 7     | Salvar CSV          | Grava todas as estações em `estacoes.csv` no formato descrito abaixo.                           |
| 8     | Carregar CSV        | Lê `estacoes.csv` e acrescenta as estações ao vetor em memória. Estatísticas são recalculadas.  |
| 0     | Sair                | Libera toda a memória dinâmica (`liberarTudo`) antes de encerrar o programa.                    |

## Função recursiva obrigatória

A função recursiva exigida pelo enunciado é:

- **`float calcularMediaRecursiva(float *v, int n)`**
  - Declarada em `src/func/funcoes.h`
  - Implementada em `src/func/funcoes.c`

Ela calcula a média aritmética de `n` leituras seguindo o padrão de **soma
recursiva** com caso base `n == 0` → retorna `0`. A média é construída pela
recorrência:

```
media(n) = (v[n-1] + media(n-1) * (n-1)) / n
```

É chamada por `calcularEstatisticas`, que por sua vez é usada em
`adicionarEstacao` e `carregarCSV`.

## Formato do arquivo CSV

O arquivo `estacoes.csv` é gravado e lido no diretório atual de execução do
programa (geralmente a raiz do projeto).

**Cabeçalho (primeira linha):**

```
ID,Nome,Operador,Sensor,Data,N,Media,Variancia,DesvioPadrao,Leituras
```

**Uma linha por estação:**

```
<id>,<nome>,<operador>,<sensor>,<dd/mm/aaaa>,<n>,<media>,<variancia>,<desvioPadrao>,<l1>;<l2>;...;<ln>
```

Detalhes do formato:

- Colunas separadas por **vírgula** (`,`).
- A data é gravada como **`dd/mm/aaaa`** (com zeros à esquerda).
- A última coluna (`Leituras`) traz os `n` valores separados por
  **ponto-e-vírgula** (`;`), para não conflitar com a vírgula do CSV.
  Assim toda a série fica em uma única célula em planilhas.
- As estatísticas (`Media`, `Variancia`, `DesvioPadrao`) são gravadas com
  4 casas decimais. Na leitura do CSV, esses campos são **recalculados** a
  partir das leituras para garantir consistência.

**Exemplo de linha:**

```
1,Norte,Joao,DHT11,15/03/2026,3,20.0000,66.6667,8.1650,10.0000;20.0000;30.0000
```

## Tipos principais (em `funcoes.h`)

- `struct DataLeitura`: dia, mês, ano da leitura.
- `struct Estacao`: id, nome (até 39 caracteres), operador (até 39),
  sensor (até 19), n, ponteiro `leituras` (vetor dinâmico), media,
  variancia, desvioPadrao e `data`.

## Validações e cuidados

- **Data:** apenas datas existentes, com ano entre **1900 e 2100**;
  considera anos bissextos.
- **ID:** rejeita IDs duplicados no cadastro.
- **N:** entre **1 e 9999** leituras por estação.
- **Alocação:** sempre checa retorno de `malloc` (NULL) antes de usar.
- **Liberação de memória:** `removerEstacao` libera a estação removida;
  `liberarTudo` libera tudo ao sair (opção 0).
- **Entrada não numérica no menu:** descarta a linha para evitar loop
  infinito do `scanf`.

## Limitações conhecidas

- Capacidade máxima fixa de **100 estações** simultâneas (vetor estático
  em `main.c`).
- Nome, operador e sensor são lidos com `%s`, então **não aceitam
  espaços** (uma palavra apenas por campo).
- A carga via CSV (opção 8) **acrescenta** ao vetor existente — não
  substitui. Recarregar duas vezes seguidas pode gerar IDs duplicados.
