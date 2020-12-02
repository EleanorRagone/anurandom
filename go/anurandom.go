package Anurandom

// Go wrapper for ANU's Quantum Random Number Generator API
// Ref: https://qrng.anu.edu.au/contact/api-documentation/

import (
    "encoding/json"
    "io/ioutil"
    "net/http"
    "time"
    "fmt"
    "log"
)

const base_url = "https://qrng.anu.edu.au/API/jsonI.php?"

// Declare structs to hold JSON response values
type Number struct {
    Data_type string 
    Length int
    Data []int
    Success bool
}

type Hex struct {
    Data_type string
    Length int
    size int
    Data []string
    Success bool
}

// Function make HTTP/GET request to ANU's API 
func callAPI(query string) ([]byte, error) {
    
    // Set HTTP timeout
    client := http.Client{
		Timeout: 30 * time.Second,
	}

    // Make HTTP/GET request
    res, _ := client.Get(query)
    if res.StatusCode != http.StatusOK {
        return nil, fmt.Errorf("Request was not OK: %v", res.Status)
    }    
    
    // Read body of HTTP response into byte array for processing
    body, err := ioutil.ReadAll(res.Body)
    if err != nil {
        return nil, fmt.Errorf("Unable to handle json response from ANU API: %v", err)
    }

    return body, nil
}

// Function to make HTTP/GET request to ANU's API, process JSON response for random numbers
func (number *Number) Generate(array_length int, data_type string) {
    
    // Build query URL
    res, err := callAPI(fmt.Sprintf("%slength=%d&type=%s", base_url, array_length, data_type))
    if err != nil {
        log.Println("ERROR: unable to call ANU API: ", err)
    }

    // Store JSON response parameters into struct object
    err = json.Unmarshal(res, &number)
    if err != nil {
        log.Println("ERROR: Unable to read json response ANU API", err)
    }
}

// Function to make HTTP/GET request to anurandom API, process JSON response for random hex strings
func (hex *Hex) Generate(array_length int, block_size int) {
    
    // Build query URL and make call to ANU API
    res, err := callAPI(fmt.Sprintf("%slength=%d&type=hex16&size=%d", base_url, array_length, block_size))
    if err != nil {
        log.Println("ERROR: unable to call ANU API: ", err)
    }
    
    // Store JSON response parameters into struct object
    err = json.Unmarshal(res, &hex)
    if err != nil {
        log.Println("ERROR: Unable to read json response ANU API", err)
    }
}
