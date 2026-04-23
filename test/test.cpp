// #include <catch2/catch_test_macros.hpp>
// #include <iostream>
// #include "CampusCompass.h"
//
// using namespace std;
//
// TEST_CASE("unsuccessful command executions", "[unsuccessful]"){
//
// 	//insert function
// 	SECTION("invalid UFID length") {
// 		CampusCompass s;
// 		s.ParseCSV("edges.csv", "classes.csv");
// 		REQUIRE(s.ParseCommand("insert \"Ayu\" 100 1 1 COP3502") == "unsuccessful");
// 	};
//
// 	//insert function
// 	SECTION("UFID contains letters") {
// 		CampusCompass s;
// 		s.ParseCSV("edges.csv", "classes.csv");
// 		REQUIRE(s.ParseCommand("insert \"Ayu\" AAAA1234 1 1 COP3502") == "unsuccessful");
// 	};
//
// 	//insert function
// 	SECTION("invalid name with numbers") {
// 		CampusCompass s;
// 		s.ParseCSV("edges.csv", "classes.csv");
// 		REQUIRE(s.ParseCommand("insert \"Ay1\" 10000001 1 1 COP3502") == "unsuccessful");
// 	};
//
// 	//insert function
// 	SECTION("invalid name with symbols") {
// 		CampusCompass s;
// 		s.ParseCSV("edges.csv", "classes.csv");
// 		REQUIRE(s.ParseCommand("insert \"A@@\" 10000001 1 1 COP3502") == "unsuccessful");
// 	};
//
// 	//insert function
// 	SECTION("invalid insert - bad class") {
// 		CampusCompass s;
// 		s.ParseCSV("edges.csv", "classes.csv");
// 		REQUIRE(s.ParseCommand("insert \"Ayushi\" 10000001 1 1 ABC1234") == "unsuccessful");
// 	};
//
// 	// drop invalid student
// 	SECTION("drop class for nonexistent student") {
// 		CampusCompass s;
// 		s.ParseCSV("edges.csv", "classes.csv");
// 		REQUIRE(s.ParseCommand("dropClass 99999999 COP3502") == "unsuccessful");
// 	};
//
// 	// remove invalid student
// 	SECTION("remove nonexistent student") {
// 		CampusCompass s;
// 		s.ParseCSV("edges.csv", "classes.csv");
// 		REQUIRE(s.ParseCommand("remove 99999999") == "unsuccessful");
// 	};
//
// 	// remove invalid class from valid student
// 	SECTION("replace invalid class") {
// 		CampusCompass s;
// 		s.ParseCSV("edges.csv", "classes.csv");
// 		s.ParseCommand("insert \"Ayu\" 10000002 1 1 COP3502");
// 		REQUIRE(s.ParseCommand("replaceClass 10000002 COP3502 BAD9999") == "unsuccessful");
// 	};
// }
//
// TEST_CASE("edge cases", "[edge]"){
//
// 	SECTION("remove non existent student") {
// 		CampusCompass s;
// 		s.ParseCSV("edges.csv", "classes.csv");
// 		REQUIRE(s.ParseCommand("remove 99999999") == "unsuccessful");
// 	};
//
// 	SECTION("removeClass with invalid class code") {
// 		CampusCompass s;
// 		s.ParseCSV("edges.csv", "classes.csv");
// 		REQUIRE(s.ParseCommand("removeClass BAD1111") == "unsuccessful");
// 	};
//
// 	SECTION("toggle non-existent edge") {
// 		CampusCompass s;
// 		s.ParseCSV("edges.csv", "classes.csv");
// 		REQUIRE(s.ParseCommand("toggleEdgesClosure 1 999 888") == "unsuccessful");
// 	};
//
// 	SECTION("check edge status of invalid edge") {
// 		CampusCompass s;
// 		s.ParseCSV("edges.csv", "classes.csv");
// 		REQUIRE(s.ParseCommand("checkEdgeStatus 100 200") == "unsuccessful");
// 	};
//
// 	SECTION("isConnected on invalid node") {
// 		CampusCompass s;
// 		s.ParseCSV("edges.csv", "classes.csv");
// 		REQUIRE(s.ParseCommand("isConnected 999 1") == "unsuccessful");
// 	};
// }
//
// TEST_CASE("core commands", "[core]"){
//
// 	SECTION("dropClass") {
// 		CampusCompass s;
// 		s.ParseCSV("edges.csv", "classes.csv");
// 		s.ParseCommand("insert \"Ayu\" 12345678 1 2 COP3502 MAC2311");
// 		REQUIRE(s.ParseCommand("dropClass 12345678 COP3502") == "successful");
// 	};
//
// 	SECTION("replaceClass") {
// 		CampusCompass s;
// 		s.ParseCSV("edges.csv", "classes.csv");
// 		s.ParseCommand("insert \"Ayu\" 12345678 1 2 COP3502 MAC2311");
// 		REQUIRE(s.ParseCommand("replaceClass 12345678 COP3502 COP3503") == "successful");
// 	};
//
// 	SECTION("removeClass") {
// 		CampusCompass s;
// 		s.ParseCSV("edges.csv", "classes.csv");
// 		s.ParseCommand("insert \"Ayu\" 10000001 1 2 COP3502 MAC2311");
// 		REQUIRE(s.ParseCommand("removeClass COP3502") == "successful");
// 	};
//
// 	SECTION("remove student") {
// 		CampusCompass s;
// 		s.ParseCSV("edges.csv", "classes.csv");
// 		s.ParseCommand("insert \"Ayu\" 10000001 1 2 COP3502 MAC2311");
// 		REQUIRE(s.ParseCommand("remove 10000001") == "successful");
// 	};
//
//
// }
//
//
// TEST_CASE("printShortestEdges ", "[printShortestEdges]"){
// 	SECTION("can access before edge closure") {
// 		CampusCompass s;
// 		s.ParseCSV("edges.csv", "classes.csv");
// 		s.ParseCommand("insert \"Ayu\" 10000001 23 1 COP3502");
// 		REQUIRE(s.ParseCommand("printShortestEdges 10000001") == "successful");
// 	};
//
// 	SECTION("cannot access after edge closure") {
// 		CampusCompass s;
// 		s.ParseCSV("edges.csv", "classes.csv");
// 		s.ParseCommand("insert \"Ayu\" 10000001 23 1 COP3502");
// 		s.ParseCommand("toggleEdgeClosure 1 23 14 23");
// 		REQUIRE(s.ParseCommand("printShortestEdges 10000001") == "unsuccessful");
// 	};
//
// }
//
