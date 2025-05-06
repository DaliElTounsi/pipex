#!/bin/bash

# ========== CONFIGURATION ==========
PIPEX="./pipex"
SRC_FILE="./src/pipex.c"
TEST_DIR="pipex_tests"
PIPEX_OUT="$TEST_DIR/pipex_out.txt"
BASH_OUT="$TEST_DIR/bash_out.txt"
PIPEX_REAL_OUT="$TEST_DIR/pipex_real_out.txt"
BASH_REAL_OUT="$TEST_DIR/bash_real_out.txt"
FORBIDDEN="$TEST_DIR/no_access.txt"
HUGE_FILE="$TEST_DIR/huge_file.txt"
EMPTY_FILE="$TEST_DIR/empty_file.txt"
SPECIAL_CHARS="$TEST_DIR/special_chars.txt"

# ========== COULEURS ==========
RED="\033[1;31m"
GREEN="\033[1;32m"
YELLOW="\033[1;33m"
BLUE="\033[1;34m"
MAGENTA="\033[1;35m"
CYAN="\033[1;36m"
RESET="\033[0m"

# ========== PRÉPARATION ==========
# Vérification de l'existence des fichiers nécessaires
if [ ! -f "$PIPEX" ]; then
    echo -e "${RED}Erreur: Le fichier exécutable pipex n'existe pas.${RESET}"
    exit 1
fi

if [ ! -f "$SRC_FILE" ]; then
    echo -e "${RED}Erreur: Le fichier source n'existe pas.${RESET}"
    exit 1
fi

# Création du répertoire de test
mkdir -p "$TEST_DIR"

# Création des fichiers spéciaux pour les tests
touch "$FORBIDDEN"
chmod 000 "$FORBIDDEN" 2>/dev/null || true

# Fichier vide
touch "$EMPTY_FILE"

# Fichier avec caractères spéciaux
echo "Ligne avec des caractères spéciaux : é è à ç ù € $ \\ \" ' \` | > < ; & * ? ! " > "$SPECIAL_CHARS"
echo "Second ligne avec des #$%^&*() symboles" >> "$SPECIAL_CHARS"

# Fichier volumineux
dd if=/dev/urandom bs=1M count=5 | base64 > "$HUGE_FILE" 2>/dev/null

# Initialisation des fichiers de sortie
> "$PIPEX_OUT"
> "$BASH_OUT"
> "$PIPEX_REAL_OUT"
> "$BASH_REAL_OUT"

# Compteur de tests et résultats
TOTAL_TESTS=0
PASSED_TESTS=0

# ========== FONCTIONS ==========
print_header() {
    echo -e "\n${CYAN}================= TEST $TOTAL_TESTS =================\n=== $1 ===${RESET}"
}

print_test_info() {
    local infile="$1"
    local cmd1="$2"
    local cmd2="$3"
    local outfile_bash="$4"
    local outfile_pipex="$5"

    echo -e "${YELLOW}Commande bash :${RESET} < $infile $cmd1 | $cmd2 > $outfile_bash"
    echo -e "${MAGENTA}Commande pipex:${RESET} $PIPEX $infile \"$cmd1\" \"$cmd2\" $outfile_pipex"
}

print_result() {
    local status=$1
    local message=$2

    if [ $status -eq 0 ]; then
        echo -e "${GREEN}✓ $message${RESET}"
        return 0
    else
        echo -e "${RED}✗ $message${RESET}"
        return 1
    fi
}

compare_outputs() {
    local test_passed=0

    if diff -q "$PIPEX_REAL_OUT" "$BASH_REAL_OUT" >/dev/null; then
        print_result 0 "Les sorties réelles sont identiques"
        test_passed=$((test_passed+1))
    else
        print_result 1 "Les sorties réelles diffèrent:"
        echo -e "${RED}Différences:${RESET}"
        diff "$BASH_REAL_OUT" "$PIPEX_REAL_OUT"
    fi

    # Vérifier aussi les fichiers de sortie d'origine si accessibles
    if [ -r "$PIPEX_OUT" ] && [ -r "$BASH_OUT" ]; then
        if diff -q "$PIPEX_OUT" "$BASH_OUT" >/dev/null; then
            print_result 0 "Les fichiers de sortie sont identiques"
            test_passed=$((test_passed+1))
        else
            print_result 1 "Les fichiers de sortie diffèrent:"
            echo -e "${YELLOW}Contenu BASH_OUT (premières lignes):${RESET}"
            head -n 5 "$BASH_OUT"
            echo -e "${YELLOW}Contenu PIPEX_OUT (premières lignes):${RESET}"
            head -n 5 "$PIPEX_OUT"
        fi
    fi

    return $test_passed
}

compare_return_codes() {
    local pipex_code=$1
    local bash_code=$2

    echo -e "Code de retour pipex: $pipex_code"
    echo -e "Code de retour bash : $bash_code"

    if [ "$pipex_code" -eq "$bash_code" ]; then
        print_result 0 "Les codes de retour sont identiques"
        return 0
    else
        print_result 1 "Les codes de retour diffèrent"
        return 1
    fi
}

run_test() {
    local description="$1"
    local infile="$2"
    local cmd1="$3"
    local cmd2="$4"
    local outfile="$5"

    TOTAL_TESTS=$((TOTAL_TESTS+1))
    print_header "$description"

    # Réinitialiser les fichiers de sortie réels pour ce test
    > "$PIPEX_REAL_OUT"
    > "$BASH_REAL_OUT"

    # Nous utilisons des fichiers de sortie différents pour bash et pipex
    local bash_out_file="${outfile}_bash"
    local pipex_out_file="${outfile}_pipex"
    print_test_info "$infile" "$cmd1" "$cmd2" "$bash_out_file" "$pipex_out_file"

    # Exécution de pipex
    local pipex_err="$TEST_DIR/pipex_err.txt"
    "$PIPEX" "$infile" "$cmd1" "$cmd2" "$pipex_out_file" > "$PIPEX_REAL_OUT" 2> "$pipex_err"
    local pipex_code=$?

    # Exécution de la commande bash équivalente
    local bash_err="$TEST_DIR/bash_err.txt"
    bash -c "< \"$infile\" $cmd1 | $cmd2 > \"$bash_out_file\"" > "$BASH_REAL_OUT" 2> "$bash_err"
    local bash_code=$?

    # Comparaison des résultats
    compare_return_codes $pipex_code $bash_code
    local output_comparison=$(compare_outputs)

    # Incrémenter PASSED_TESTS uniquement si les deux comparaisons réussissent
    if [ $? -eq 0 ] && [ $output_comparison -eq 2 ]; then
        PASSED_TESTS=$((PASSED_TESTS+1))
    fi

    # Affichage des erreurs si présentes
    if [ -s "$pipex_err" ]; then
        echo -e "${YELLOW}Erreurs pipex:${RESET}"
        cat "$pipex_err"
    fi

    if [ -s "$bash_err" ]; then
        echo -e "${YELLOW}Erreurs bash:${RESET}"
        cat "$bash_err"
    fi

    echo -e "${BLUE}------------------------------------------${RESET}"
}

run_multi_pipe_test() {
    local description="$1"
    local infile="$2"
    local commands="$3"
    local outfile="$4"

    TOTAL_TESTS=$((TOTAL_TESTS+1))
    print_header "$description"

    # Réinitialiser les fichiers de sortie pour ce test
    > "$PIPEX_REAL_OUT"
    > "$BASH_REAL_OUT"

    # Nous utilisons des fichiers de sortie différents pour bash et pipex
    local bash_out_file="${outfile}_bash"
    local pipex_out_file="${outfile}_pipex"

    # Format correct pour les commandes bash avec redirection d'entrée
    local bash_cmd="< $infile $commands > $bash_out_file"

    # Extraction des commandes individuelles pour pipex
    IFS='|' read -ra CMD_ARRAY <<< "$commands"

    # Formatage des commandes pour pipex avec guillemets autour de chaque commande
    local pipex_cmds=""
    for cmd in "${CMD_ARRAY[@]}"; do
        pipex_cmds="$pipex_cmds \"$(echo $cmd | xargs)\""
    done

    # Afficher les commandes à exécuter
    echo -e "${YELLOW}Commande bash :${RESET} $bash_cmd"
    echo -e "${MAGENTA}Commande pipex:${RESET} $PIPEX $infile$pipex_cmds $pipex_out_file"

    # Exécution de la commande bash avec pipes multiples
    local bash_err="$TEST_DIR/bash_err.txt"
    bash -c "$bash_cmd" > "$BASH_REAL_OUT" 2> "$bash_err"
    local bash_code=$?

    # Exécution de pipex
    local pipex_err="$TEST_DIR/pipex_err.txt"
    eval "$PIPEX $infile$pipex_cmds $pipex_out_file" > "$PIPEX_REAL_OUT" 2> "$pipex_err"
    local pipex_code=$?

    # Comparaison des résultats
    echo -e "Code de retour pipex: $pipex_code"
    echo -e "Code de retour bash : $bash_code"

    if [ "$pipex_code" -eq "$bash_code" ]; then
        print_result 0 "Les codes de retour sont identiques"
    else
        print_result 1 "Les codes de retour diffèrent"
    fi

    # Comparaison des sorties
    if diff -q "$PIPEX_REAL_OUT" "$BASH_REAL_OUT" >/dev/null; then
        print_result 0 "Les sorties réelles sont identiques"
        PASSED_TESTS=$((PASSED_TESTS+1))
    else
        print_result 1 "Les sorties réelles diffèrent:"
        echo -e "${RED}Différences:${RESET}"
        diff "$BASH_REAL_OUT" "$PIPEX_REAL_OUT"
    fi

    # Vérifier aussi les fichiers de sortie si accessibles
    if [ -r "$pipex_out_file" ] && [ -r "$bash_out_file" ]; then
        if diff -q "$pipex_out_file" "$bash_out_file" >/dev/null; then
            print_result 0 "Les fichiers de sortie sont identiques"
        else
            print_result 1 "Les fichiers de sortie diffèrent:"
            echo -e "${YELLOW}Contenu BASH_OUT (premières lignes):${RESET}"
            head -n 5 "$bash_out_file"
            echo -e "${YELLOW}Contenu PIPEX_OUT (premières lignes):${RESET}"
            head -n 5 "$pipex_out_file"
        fi
    fi

    # Affichage des erreurs si présentes
    if [ -s "$pipex_err" ]; then
        echo -e "${YELLOW}Erreurs pipex:${RESET}"
        cat "$pipex_err"
    fi

    if [ -s "$bash_err" ]; then
        echo -e "${YELLOW}Erreurs bash:${RESET}"
        cat "$bash_err"
    fi

    echo -e "${BLUE}------------------------------------------${RESET}"
}

# ========== TESTS DE BASE ==========
# Test 1: Commandes simples
run_test "Test basique: cat | cat" "$SRC_FILE" "cat" "cat" "$TEST_DIR/test1"

# Test 2: Utilisation de wc
run_test "Test avec wc: ls | wc -l" "$TEST_DIR" "ls" "wc -l" "$TEST_DIR/test2"

# Test 3: Chemins absolus
run_test "Test chemins absolus" "$SRC_FILE" "/bin/cat" "/usr/bin/head -n 5" "$TEST_DIR/test3"

# ========== TESTS D'ERREUR CLASSIQUES ==========
# Test 4: Commande invalide
run_test "Test commande invalide: ls | commande_invalide" "$SRC_FILE" "ls" "commande_invalide" "$TEST_DIR/test4"

# Test 5: Première commande invalide
run_test "Test première commande invalide: commande_invalide | wc" "$SRC_FILE" "commande_invalide" "wc" "$TEST_DIR/test5"

# Test 6: Fichier d'entrée inexistant
run_test "Test fichier d'entrée inexistant" "fichier_inexistant.txt" "cat" "wc -l" "$TEST_DIR/test6"

# Test 7: Fichier de sortie sans permission
run_test "Test fichier de sortie sans permission" "$SRC_FILE" "ls" "wc -l" "$FORBIDDEN"

# Test 8: Fichier d'entrée sans permission
run_test "Test fichier d'entrée sans permission" "$FORBIDDEN" "cat" "wc -l" "$TEST_DIR/test8"

# Test 9: Commande vide
run_test "Test commande vide" "$SRC_FILE" "" "wc -l" "$TEST_DIR/test9"

# ========== TESTS AVANCÉS ==========
# Test 10: Commandes avec arguments multiples
run_test "Test arguments multiples: grep -i include | sort" "$SRC_FILE" "grep -i include" "sort" "$TEST_DIR/test10"

# Test 11: Commandes avec guillemets
run_test "Test avec guillemets: grep \"int main\"" "$SRC_FILE" "grep \"int main\"" "wc -l" "$TEST_DIR/test11"

# Test 12: Commandes avec caractères d'échappement
run_test "Test avec caractères d'échappement" "$SRC_FILE" "grep \\\"\\\\" "wc -l" "$TEST_DIR/test12"

# Test 13: Fichier vide
run_test "Test avec fichier vide" "$EMPTY_FILE" "cat" "wc -l" "$TEST_DIR/test13"

# Test 14: Caractères spéciaux
run_test "Test avec caractères spéciaux" "$SPECIAL_CHARS" "cat" "grep caractères" "$TEST_DIR/test14"

# Test 15: Redirection dans le même fichier
run_test "Test redirection dans le même fichier" "$SRC_FILE" "cat" "wc -l" "$SRC_FILE"

# Test 16: Gros fichier
run_test "Test avec gros fichier" "$HUGE_FILE" "cat" "head -n 10" "$TEST_DIR/test16"

# Test 17: Sortie binaire
run_test "Test avec sortie binaire" "$SRC_FILE" "xxd" "head -n 10" "$TEST_DIR/test17"

# Test 18: Commande avec variables d'environnement
run_test "Test avec variables d'environnement" "$SRC_FILE" "env" "grep PATH" "$TEST_DIR/test18"

# Test 19: Commande avec exit code non zéro
run_test "Test avec exit code non zéro" "$SRC_FILE" "grep pattern_inexistant" "wc -l" "$TEST_DIR/test19"

# Test 20: Commande avec beaucoup d'arguments
long_args=""
for i in {1..100}; do
    long_args="$long_args $i"
done
run_test "Test avec beaucoup d'arguments" "$SRC_FILE" "echo $long_args" "wc -w" "$TEST_DIR/test20"

# Test 21: Test de timeout
run_test "Test de timeout" "$SRC_FILE" "sleep 0.5" "cat" "$TEST_DIR/test21"

# Test 22: Test avec /dev/null
run_test "Test avec /dev/null" "/dev/null" "cat" "wc -l" "$TEST_DIR/test22"

# Test 23: Test avec /dev/random (limité)
run_test "Test avec /dev/random (limité)" "/dev/random" "head -c 100" "xxd" "$TEST_DIR/test23"

# Test 24: Test avec commande qui génère des erreurs
run_test "Test avec commande générant des erreurs" "$SRC_FILE" "ls fichier_inexistant" "grep error" "$TEST_DIR/test24"

# Test 25: Test avec espaces dans les arguments
run_test "Test avec espaces dans arguments" "$SRC_FILE" "grep \"int \" " "wc -l" "$TEST_DIR/test25"

# ========== TESTS MULTI-PIPES ==========
# Test de base à 3 commandes
run_multi_pipe_test "Test multi-pipes: cat | grep | wc" \
    "$SRC_FILE" \
    "cat | grep include | wc -l" \
    "$TEST_DIR/multi1"

# Test complexe à 5 commandes
run_multi_pipe_test "Test multi-pipes complexe" \
    "$SRC_FILE" \
    "cat | grep int | sort | uniq | wc -l" \
    "$TEST_DIR/multi2"

# Test avec commande générant des erreurs
run_multi_pipe_test "Test multi-pipes avec erreur" \
    "$SRC_FILE" \
    "cat | grep non_existant | sort | wc -l" \
    "$TEST_DIR/multi3"

# Test avec redirections intermédiaires (simplifié)
run_multi_pipe_test "Test multi-pipes avec redirections" \
    "$SRC_FILE" \
    "cat | grep -v '//' | grep include | sort" \
    "$TEST_DIR/multi4"

# Test avec caractères spéciaux
run_multi_pipe_test "Test multi-pipes avec caractères spéciaux" \
    "$SPECIAL_CHARS" \
    "cat | grep -i 'caractères' | grep -v 'symboles' | wc -l" \
    "$TEST_DIR/multi5"

# Test avec variables d'environnement
run_multi_pipe_test "Test multi-pipes avec variables d'environnement" \
    "/dev/null" \
    "env | grep PATH | sort | head -n 3" \
    "$TEST_DIR/multi6"

# Test avec commandes complexes
run_multi_pipe_test "Test multi-pipes avec commandes complexes" \
    "." \
    "find . -type f -name \"*.c\" | xargs cat 2>/dev/null | grep -i \"include\" | sort | uniq | wc -l" \
    "$TEST_DIR/multi7"

# ========== TEST DE ZOMBIE ==========
print_header "Test de zombie processes"
echo -e "${MAGENTA}Test avec: sleep 5 | sleep 1${RESET}"

echo "Exécution avec pipex:"
time "$PIPEX" "$SRC_FILE" "sleep 5" "sleep 1" "$TEST_DIR/test_sleep_pipex"

echo "Exécution avec bash:"
time bash -c "< $SRC_FILE sleep 5 | sleep 1 > $TEST_DIR/test_sleep_bash"

# ========== TEST DE LEAK MÉMOIRE ==========
if command -v valgrind &> /dev/null; then
    print_header "Test de leak mémoire avec valgrind"
    echo -e "${MAGENTA}Exécution de valgrind:${RESET}"
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose "$PIPEX" "$SRC_FILE" "cat" "wc -l" "$TEST_DIR/test_valgrind"
else
    echo -e "${YELLOW}Valgrind n'est pas installé. Test de leak mémoire ignoré.${RESET}"
fi

# ========== RÉCAPITULATION DES RÉSULTATS ==========
echo -e "\n${CYAN}================= RÉSUMÉ =================\n"
echo -e "${YELLOW}Tests exécutés: $TOTAL_TESTS"
echo -e "${GREEN}Tests réussis: $PASSED_TESTS"

# Calcul correct du nombre de tests échoués
FAILED_TESTS=$((TOTAL_TESTS - PASSED_TESTS))
echo -e "${RED}Tests échoués: $FAILED_TESTS${RESET}"

# Calcul du pourcentage de réussite
if [ $TOTAL_TESTS -gt 0 ]; then
    PERCENT=$((PASSED_TESTS * 100 / TOTAL_TESTS))
    echo -e "${CYAN}Taux de réussite: $PERCENT%${RESET}"
else
    echo -e "${RED}Aucun test exécuté.${RESET}"
fi

if [ $PERCENT -eq 100 ]; then
    echo -e "${GREEN}🎉 FÉLICITATIONS! Tous les tests sont passés avec succès! 🎉${RESET}"
elif [ $PERCENT -ge 80 ]; then
    echo -e "${GREEN}👍 Bon travail! La plupart des tests sont passés. 👍${RESET}"
elif [ $PERCENT -ge 50 ]; then
    echo -e "${YELLOW}🤔 Plus de la moitié des tests sont passés. Continuez à travailler! 🤔${RESET}"
else
    echo -e "${RED}😱 Moins de la moitié des tests sont passés. Il reste du travail! 😱${RESET}"
fi

# ========== NETTOYAGE ==========
chmod 644 "$FORBIDDEN" 2>/dev/null || true
echo -e "\n${GREEN}Tous les tests sont terminés!${RESET}"
