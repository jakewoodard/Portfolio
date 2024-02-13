// Kenneth Woodard
// PA4
// COSC 365

package main

import (
	"fmt"
	"io"
	"os"
	"path/filepath"
	"sync"
)

func main() {
	if len(os.Args) < 3 {
		fmt.Fprintf(os.Stderr, "usage: %s <file1> <file2> ... <destination_directory>\n", os.Args[0])
		os.Exit(1)
	}

	srcFiles := os.Args[1 : len(os.Args)-1]
	destDir := os.Args[len(os.Args)-1]
	if _, err := os.Stat(destDir); os.IsNotExist(err) || !isDirectory(destDir) {
		fmt.Fprintf(os.Stderr, "error: destination directory %s does not exist or is not a directory\n", destDir)
		os.Exit(1)
	}

	errCh := make(chan error, len(srcFiles))
	var wg sync.WaitGroup
	wg.Add(len(srcFiles))
	for _, file := range srcFiles {
		go func(file string) {
			defer wg.Done()
			if _, err := os.Stat(file); os.IsNotExist(err) || isDirectory(file) {
				errCh <- fmt.Errorf("file %s does not exist or is a directory", file)
				return
			}

			in, err := os.Open(file)
			if err != nil {
				errCh <- fmt.Errorf("failed to open file %s for reading: %v", file, err)
				return
			}

			defer in.Close()
			fileName := filepath.Base(file)
			destFile := filepath.Join(destDir, fileName)
			out, err := os.Create(destFile)
			if err != nil {
				errCh <- fmt.Errorf("failed to create file %s in %s: %v", fileName, destDir, err)
				return
			}

			defer out.Close()
			if _, err := io.Copy(out, in); err != nil {
				errCh <- fmt.Errorf("failed to copy file %s to %s: %v", file, destFile, err)
				return
			}
			
			errCh <- nil
			fmt.Printf("%s copied\n", fileName)
		}(file)
	}

	go func() {
		wg.Wait()
		close(errCh)
	}()

	for err := range errCh {
		if err != nil {
			fmt.Fprintf(os.Stderr, "error: %v\n", err)
			os.Exit(1)
		}
	}
}

func isDirectory(name string) bool {
	if fileInfo, err := os.Stat(name); err == nil {
		return fileInfo.Mode().IsDir()
	}
	return false
}

