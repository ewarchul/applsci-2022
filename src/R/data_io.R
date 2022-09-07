library(tidyverse)
library(magrittr)

read_solver_data <- function(datadir, solver_info) {
  mk_datapaths(solver_info$dims, solver_info$models) |>
    read_data(datadir)
}

read_data <- function(datapaths, datadir) {
  datapaths |>
    purrr::map(\(dp) {
      readr::read_tsv(
        here::here(datadir, dp["path"]),
        col_names = FALSE,
      ) |>
        dplyr::mutate(
          NetworkSize = as.numeric(dp["size"]),
          Model = factor(dp["model"])
        )
    }) |>
    purrr::reduce(dplyr::bind_rows)
}

prepare_df <- function(dfx, reps, max_iters) {
  dfx |>
    dplyr::group_by(Model, NetworkSize) |>
    dplyr::group_modify(\(df, g) {
      df |>
        split_reps(reps, max_iters) |>
        label_reps() |>
        purrr::reduce(dplyr::bind_rows)
    }) |>
    dplyr::ungroup()
}

split_reps <- function(dfx, reps = 10, max_iters = 1700) {
  dfs <- list()
  for (r in 0:reps) {
    ids <- seq(r * max_iters + 1, max_iters * (r + 1))
    dfs[[r + 1]] <- dfx[ids, ]
  }
  dfs
}

label_reps <- function(dfx) {
  1:length(dfx) |> purrr::map(\(rep) {
    dfx[[rep]] <- dfx[[rep]] |> dplyr::mutate(Repetition = rep)
  })
}

mk_datapaths <- function(dims, models) {
  expand.grid(m = models, d = dims) |> purrr::pmap(\(m, d) {
    if (d < 10) {
      c(
        model = stringr::str_glue("{m}"),
        size = as.numeric(d),
        path = stringr::str_glue("{m}/0{d}v.csv")
      )
    } else {
      c(
        model = stringr::str_glue("{m}"),
        size = as.numeric(d),
        path = stringr::str_glue("{m}/{d}v.csv")
      )
    }
  })
}

tidy_data <- function(df) {
  df |>
    dplyr::rename(Generation = X1, Cost = X2) |>
    tidyr::drop_na()
}

BA_info <- list(
  models = c("BA_fm", "BA_sm"),
  max_iters = 1700,
  dims = c(30),
  reps = 10
)

EA_info <- list(
  models = c("EA_fm", "EA_sm"),
  max_iters = 50000,
  dims = c(30),
  reps = 10
)

mk_ba_dataset <- function() {
  "data/extdata" %>%
    read_solver_data(BA_info) %>%
    prepare_df(BA_info$reps, BA_info$max_iters) %>%
    tidy_data()
}

mk_ea_dataset <- function() {
  "data/extdata" %>%
    read_solver_data(EA_info) %>%
    prepare_df(EA_info$reps, EA_info$max_iters) %>%
    tidy_data()
}
