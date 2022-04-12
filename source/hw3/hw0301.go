package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"os"
	"strings"
)

func main() {
	bible_bytes, err := ioutil.ReadFile("bible.txt")
	if err != nil {
		panic(err)
	}

	bible_raw := strings.Split(string(bible_bytes), "\n")
	bible := make([]Item, 0, len(bible_raw))
	for _, line := range bible_raw {
		if len(line) == 0 {
			continue
		}
		item := Item{}
		if err := json.Unmarshal([]byte(line), &item); err != nil {
			panic(err)
		}
		bible = append(bible, item)
	}

	var target string
	fmt.Print("Please enter the search target: ")
	scanner := bufio.NewScanner(os.Stdin)
	scanner.Scan()
	target = scanner.Text()

	result := make([]Item, 0, len(bible))
	for _, item := range bible {
		if strings.Contains(strings.ToLower(item.Text), strings.ToLower(target)) {
			result = append(result, item)
		}
	}

	fmt.Println("Found", len(result), "time(s)")
	for i, item := range result {
		fmt.Printf("%d. %s %d:%d %s\n", i+1, item.BookID, item.Chapter, item.Verse, item.Text)
	}
}

type Item struct {
	Chapter       int    `json:"chapter"`
	Verse         int    `json:"verse"`
	Text          string `json:"text"`
	TranslationID string `json:"translation_id"`
	BookID        string `json:"book_id"`
	BookName      string `json:"book_name"`
}

// Here is the explanation for the code above:
// 1. We read the bible.txt file, and split it by newline
// 2. We loop over the lines, and unmarshal each line into a Item struct
// 3. We then check if the target string is included in the item's text
// 4. If yes, we append the item to the result slice
// 5. We print the result slice
