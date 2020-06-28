This is a basic sample using BPF and XDP. For build and run, go to root directory (../) and run the folowing command: 

```
make run-samples
```

Do not foget to insert the root password in order to load the BPF object file in the kernel.

## Tree

The directory was based on this [note](https://blogs.oracle.com/linux/notes-on-bpf-4).  

- The bpf folder was created to store bpf files (kernel space). 
- The current directory was create to store user files (user space).