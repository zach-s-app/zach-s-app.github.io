-- 2048 in pure Lua (terminal)
math.randomseed(os.time())

local SIZE = 4
local board = {}

-- Initialize board with zeros
local function initBoard()
  board = {}
  for i = 1, SIZE do
    board[i] = {}
    for j = 1, SIZE do
      board[i][j] = 0
    end
  end
end

-- Add a random tile (2 or 4) in an empty spot
local function addRandomTile()
  local empties = {}
  for i = 1, SIZE do
    for j = 1, SIZE do
      if board[i][j] == 0 then
        table.insert(empties, {i, j})
      end
    end
  end
  if #empties == 0 then return false end
  local pos = empties[math.random(#empties)]
  board[pos[1]][pos[2]] = (math.random() < 0.9) and 2 or 4
  return true
end

-- Print the board
local function printBoard()
  print("\n---------------------")
  for i = 1, SIZE do
    for j = 1, SIZE do
      if board[i][j] == 0 then
        io.write(string.format("%4s", "."))
      else
        io.write(string.format("%4d", board[i][j]))
      end
    end
    print()
  end
  print("---------------------")
end

-- Slide and merge one row or column array left
local function slideAndMerge(line)
  local newLine = {}
  -- Remove zeros
  for _, v in ipairs(line) do
    if v ~= 0 then table.insert(newLine, v) end
  end
  -- Merge tiles
  local i = 1
  while i < #newLine do
    if newLine[i] == newLine[i+1] then
      newLine[i] = newLine[i] * 2
      table.remove(newLine, i+1)
    end
    i = i + 1
  end
  -- Fill with zeros to maintain length
  while #newLine < SIZE do
    table.insert(newLine, 0)
  end
  return newLine
end

-- Move board left
local function moveLeft()
  local moved = false
  for i = 1, SIZE do
    local line = board[i]
    local newLine = slideAndMerge(line)
    for j = 1, SIZE do
      if board[i][j] ~= newLine[j] then
        moved = true
        board[i][j] = newLine[j]
      end
    end
  end
  return moved
end

-- Move board right
local function moveRight()
  local moved = false
  for i = 1, SIZE do
    local line = {}
    for j = SIZE, 1, -1 do
      table.insert(line, board[i][j])
    end
    local newLine = slideAndMerge(line)
    for j = SIZE, 1, -1 do
      if board[i][j] ~= newLine[SIZE - j + 1] then
        moved = true
        board[i][j] = newLine[SIZE - j + 1]
      end
    end
  end
  return moved
end

-- Move board up
local function moveUp()
  local moved = false
  for j = 1, SIZE do
    local line = {}
    for i = 1, SIZE do
      table.insert(line, board[i][j])
    end
    local newLine = slideAndMerge(line)
    for i = 1, SIZE do
      if board[i][j] ~= newLine[i] then
        moved = true
        board[i][j] = newLine[i]
      end
    end
  end
  return moved
end

-- Move board down
local function moveDown()
  local moved = false
  for j = 1, SIZE do
    local line = {}
    for i = SIZE, 1, -1 do
      table.insert(line, board[i][j])
    end
    local newLine = slideAndMerge(line)
    for i = SIZE, 1, -1 do
      if board[i][j] ~= newLine[SIZE - i + 1] then
        moved = true
        board[i][j] = newLine[SIZE - i + 1]
      end
    end
  end
  return moved
end

-- Check if moves are possible
local function canMove()
  for i = 1, SIZE do
    for j = 1, SIZE do
      if board[i][j] == 0 then return true end
      if j < SIZE and board[i][j] == board[i][j+1] then return true end
      if i < SIZE and board[i][j] == board[i+1][j] then return true end
    end
  end
  return false
end

-- Main game loop
local function main()
  initBoard()
  addRandomTile()
  addRandomTile()
  printBoard()

  while true do
    io.write("Move (w=up, a=left, s=down, d=right, q=quit): ")
    local input = io.read()
    if not input or #input == 0 then input = "q" end
    input = input:sub(1,1):lower()

    local moved = false
    if input == "w" then moved = moveUp()
    elseif input == "a" then moved = moveLeft()
    elseif input == "s" then moved = moveDown()
    elseif input == "d" then moved = moveRight()
    elseif input == "q" then
      print("Thanks for playing!")
      break
    else
      print("Invalid input!")
    end

    if moved then
      addRandomTile()
      printBoard()
      if not canMove() then
        print("Game Over! No moves left.")
        break
      end
    else
      print("No tiles moved. Try a different direction.")
    end
  end
end

main()

