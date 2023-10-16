function main() {
  const num1 = parseInt(process.argv[2])
  const num2 = parseInt(process.argv[3])

  const sum = num1 + num2

  process.stdout.write(sum.toString())
}

main()