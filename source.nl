func sum(a, b) {
    let int result;
    result = a + b;
    return result;
}

let int i;
let int total;

i = 1;
total = 0;

while (i <= 5) {
    read(total);
    total = sum(total, i);
}

print(total);