package main

// Usage example for 'anurandom' Go package

import (
    "fmt"
    "anurandom"
)

func main() {

    // Generate 10 random numbers between 0-255
    var myRandom Anurandom.Number
    myRandom.Generate(10, "uint8")

    // Returns integer slice containing 10 random numbers
    if myRandom.Success == true {
        numbers := myRandom.Data
        fmt.Println(numbers)
        fmt.Printf("Type = %T\n", numbers)
    }

    // Generate 5 random numbers between 0–65535
    var myRandomLarge Anurandom.Number
    myRandomLarge.Generate(5, "uint16")

    // Returns integer slice containing 5 random numbers
    if myRandomLarge.Success == true {
        numbersLarge := myRandomLarge.Data
        fmt.Println(numbersLarge)
        fmt.Printf("Type = %T\n", numbersLarge)
    }

    // 10 blocks of random numbers in hexadecimal format. Each block is between 0000–ffff
    var myRandomHex Anurandom.Hex
    myRandomHex.Generate(10, 4)

    // Returns string slice containing 10, 2 character hex strings
    if myRandomHex.Success == true {
        hexStrings := myRandomHex.Data
        fmt.Println(hexStrings)
        fmt.Printf("Type = %T\n", hexStrings)
    }
}
