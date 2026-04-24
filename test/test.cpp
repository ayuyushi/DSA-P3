// #include <catch2/catch_test_macros.hpp>
// #include <iostream>
// #include "CampusCompass.h"
//
// using namespace std;
//
// // five incorrect commands
//
// TEST_CASE("incorrect commands", "[incorrect]") {
//
//     SECTION("name with numbers") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         REQUIRE(s.ParseCommand("insert \"A11y\" 45679999 1 1 COP3502") == "unsuccessful");
//     }
//
//     SECTION("invalid UF ID") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         REQUIRE(s.ParseCommand("insert \"Alice\" 1234 1 1 COP3502") == "unsuccessful");
//     }
//
//     SECTION("UFID contains letters") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         REQUIRE(s.ParseCommand("insert \"Alice\" ABCD1234 1 1 COP3502") == "unsuccessful");
//     }
//
//     SECTION("name with symbols") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         REQUIRE(s.ParseCommand("insert \"Al!ce\" 10000001 1 1 COP3502") == "unsuccessful");
//     }
//
//     SECTION("invalid class") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         REQUIRE(s.ParseCommand("insert \"Alice\" 10000001 1 1 ZZZ9999") == "unsuccessful");
//     }
//
//     SECTION("wrong format of class code") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         REQUIRE(s.ParseCommand("insert \"Alice\" 10000001 1 1 cop3502") == "unsuccessful");
//     }
//
//     SECTION("insert duplicate classes") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         REQUIRE(s.ParseCommand("insert \"Alice\" 10000001 1 2 COP3502 COP3502") == "unsuccessful");
//     }
// }
//
// // edge cases
//
// TEST_CASE("edge cases", "[edge]") {
//
//     SECTION("remove non existing student") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         REQUIRE(s.ParseCommand("remove 99999999") == "unsuccessful");
//     }
//
//     SECTION("checkEdgeStatus on nonexistent edge") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         REQUIRE(s.ParseCommand("checkEdgeStatus 1 3") == "DNE");
//     }
//
//     SECTION("isConnected with invalid node") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         REQUIRE(s.ParseCommand("isConnected 999 1") == "unsuccessful");
//     }
//
//     SECTION("checking if dropping student with one class deletes student from data structure") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         s.ParseCommand("insert \"Solo\" 11000001 1 1 COP3502");
//         REQUIRE(s.ParseCommand("dropClass 11000001 COP3502") == "successful");
//         REQUIRE(s.getStudent(11000001) == nullptr);
//     }
//
//     SECTION("replaceClass with an existing class") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         s.ParseCommand("insert \"Eve\" 11000002 1 2 COP3502 MAC2311");
//         REQUIRE(s.ParseCommand("replaceClass 11000002 COP3502 MAC2311") == "unsuccessful");
//     }
// }
//
// // core commands
//
// TEST_CASE("core commands", "[core]") {
//
//     SECTION("dropClass - student with many classes") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         s.ParseCommand("insert \"Bob\" 20000001 1 2 COP3502 MAC2311");
//         REQUIRE(s.ParseCommand("dropClass 20000001 COP3502") == "successful");
//         REQUIRE(s.getStudent(20000001) != nullptr);
//     }
//
//     SECTION("dropClass - nonexisting student") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         REQUIRE(s.ParseCommand("dropClass 99999999 COP3502") == "unsuccessful");
//     }
//
//     SECTION("dropClass - nonexisting class with existing student") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         s.ParseCommand("insert \"Bob\" 20000002 1 1 MAC2311");
//         REQUIRE(s.ParseCommand("dropClass 20000002 COP3502") == "unsuccessful");
//     }
//
//     SECTION("remove - existing student") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         s.ParseCommand("insert \"Carol\" 30000001 1 1 COP3502");
//         REQUIRE(s.ParseCommand("remove 30000001") == "successful");
//         REQUIRE(s.getStudent(30000001) == nullptr);
//     }
//
//     SECTION("remove - nonexistent student") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         REQUIRE(s.ParseCommand("remove 99999999") == "unsuccessful");
//     }
//
//     SECTION("replaceClass - success") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         s.ParseCommand("insert \"Dan\" 40000001 1 1 COP3502");
//         REQUIRE(s.ParseCommand("replaceClass 40000001 COP3502 COP3503") == "successful");
//     }
//
//     SECTION("replaceClass - fail") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         s.ParseCommand("insert \"Frank\" 40000002 1 1 MAC2311");
//         REQUIRE(s.ParseCommand("replaceClass 40000002 COP3502 COP3503") == "unsuccessful");
//     }
//
//     SECTION("replaceClass - fail with invalid class") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         s.ParseCommand("insert \"Grace\" 40000003 1 1 COP3502");
//         REQUIRE(s.ParseCommand("replaceClass 40000003 COP3502 ZZZ9999") == "unsuccessful");
//     }
//
//     SECTION("removeClass - multiple students") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         s.ParseCommand("insert \"Hank\" 50000001 1 2 COP3502 MAC2311");
//         s.ParseCommand("insert \"Ivy\" 50000002 1 1 COP3502");
//         REQUIRE(s.ParseCommand("removeClass COP3502") == "successful");
//         REQUIRE(s.getStudent(50000002) == nullptr);
//         REQUIRE(s.getStudent(50000001) != nullptr);
//     }
//
//     SECTION("removeClass - no students in class") {
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         REQUIRE(s.ParseCommand("removeClass COP3503") == "unsuccessful");
//     }
// }
//
// // printShortestEdges
//
// TEST_CASE("printShortestEdges", "[shortest]") {
//
//     SECTION("reachable, close all edges, unreachable, first test") {
//         CampusCompass s;
//         s.ParseCommand("insert \"Ivan\" 80000001 1 1 COP3502");
//
//         int id = 80000001;
//         vector<string> classes = {"COP3502"};
//
//         vector<int> distBefore = s.shortestEdges(id, classes);
//         REQUIRE(distBefore[0] == 25);
//
//         REQUIRE(s.ParseCommand("toggleEdgesClosure 3 13 23 14 23 22 23") == "successful");
//         REQUIRE(s.ParseCommand("checkEdgeStatus 13 23") == "closed");
//         REQUIRE(s.ParseCommand("checkEdgeStatus 14 23") == "closed");
//         REQUIRE(s.ParseCommand("checkEdgeStatus 22 23") == "closed");
//
//         vector<int> distAfter = s.shortestEdges(id, classes);
//         REQUIRE(distAfter[0] == -1);
//
//         REQUIRE(s.ParseCommand("toggleEdgesClosure 3 13 23 14 23 22 23") == "successful");
//         vector<int> distRestored = s.shortestEdges(id, classes);
//         REQUIRE(distRestored[0] == 25);
//     }
//
//
//     SECTION("reachable, close all edges, unreachable, second test") {
//
//         CampusCompass s;
//         s.ParseCSV("data/edges.csv", "data/classes.csv");
//         s.ParseCommand("insert \"Judy\" 80000002 5 1 PHY2048");
//
//         int id = 80000002;
//         vector<string> classes = {"PHY2048"};
//
//         vector<int> distBefore = s.shortestEdges(id, classes);
//         REQUIRE(distBefore[0] == 12);
//
//         s.ParseCommand("toggleEdgesClosure 1 49 56");
//         REQUIRE(s.ParseCommand("checkEdgeStatus 49 56") == "closed");
//
//         vector<int> distAfter = s.shortestEdges(id, classes);
//         REQUIRE(distAfter[0] == -1);
//     }
// }
//
