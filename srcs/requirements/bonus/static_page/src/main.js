const gotError = (label, inputBox, warn) => {
    label.forEach((lb) => {
      lb.classList.add("error");
    })
    inputBox.forEach((box) => {
      box.classList.add("error");
    })
    warn.classList.add("error");
    reset();
}
const removeErrorAll = (label, inputBox, warning) => {
    label.forEach((lb) => {
      if (lb.classList.contains("error"))
        lb.classList.remove("error");
    })
    inputBox.forEach((box) => {
      if (box.classList.contains("error"))
        box.classList.remove("error");
    })
    warning.forEach((err) => {
      if (err.classList.contains("error"))
        err.classList.remove("error");
    })
}
const removeError = (err) => {
  if (err.classList.contains("error"))
    err.classList.remove("error");
}

const reset = () => {
  document.getElementById("yy").innerHTML = "--";
  document.getElementById("mm").innerHTML = "--";
  document.getElementById("dd").innerHTML = "--";
}

const calcAge = function(year, month, day, lastDayOfMonth) {
  const currentYear = new Date().getFullYear();
  const currentMonth = new Date().getMonth() +1;
  const currentDay = new Date().getDate();
  
  year = currentYear - year;
  month = currentMonth - month;
  day = currentDay - day;
  if (day < 0) {
    month--;
    day += lastDayOfMonth;
  }
  if (month < 0) {
    year--;
    month +=12;
  }
  document.getElementById("yy").innerHTML = year;
  document.getElementById("mm").innerHTML = month;
  document.getElementById("dd").innerHTML = day;
}

const callMe = () => {
    const day = document.getElementById("day").value;
    const month = document.getElementById("month").value;
    const year = document.getElementById("year").value;
    const inputBox = document.querySelectorAll(".input-box");
    const label = document.querySelectorAll(".title");
    const warning = document.querySelectorAll(".warning");

    day.length === 0 ? warning[0].innerHTML = "This field is required"
      : warning[0].innerHTML = "Must be a valid day";
    month.length === 0 ? warning[1].innerHTML = "This field is required"
      : warning[1].innerHTML = "Must be a valid month";
    year.length === 0 ? warning[2].innerHTML = "This field is required"
      : warning[2].innerHTML = "Must be a valid year";
    if (day < 1 || day > 31) {
      gotError(label, inputBox, warning[0]);
    } else
        removeError(warning[0]);
    if (month < 1 || month > 12) {
      gotError(label, inputBox, warning[1]);
    } else
        removeError(warning[1]);
    if (year < 1900 || year > 2023) {
      gotError(label, inputBox, warning[2]);
      return false;
    } else
        removeError(warning[2]);
    
    const lastDayOfMonth = new Date(year, month, 0).getDate();
    if (day < 1 || day > lastDayOfMonth) {
      gotError(label, inputBox, warning[0]);
        return false;
    } 
    if (!(month < 1 || month > 12)) {
      removeErrorAll(label, inputBox, warning);
      calcAge(year, month, day, lastDayOfMonth);
    }
  return false;
};
