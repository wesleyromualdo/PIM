<?php
/**
 * Classes de mapeamento para o acesso ao serviço Spo_Ws_Sof_Quantitativo.
 *
 * @version $Id: QuantitativoMap.php 134087 2017-11-20 12:57:03Z saulocorreia $
 */

/**
 * Classe de definição de mapeamentos do webservice.
 *
 * @see Spo_Ws_Sof_Quantitativo
 */
class Spo_Ws_Sof_QuantitativoMap
{
    public static $classmap = [
        'consultarEmendasLocalizador'                             => 'ConsultarEmendasLocalizador',
        'credencialDTO'                                           => 'CredencialDTO',
        'baseDTO'                                                 => 'BaseDTO',
        'localizadorDTO'                                          => 'LocalizadorDTO',
        'consultarEmendasLocalizadorResponse'                     => 'ConsultarEmendasLocalizadorResponse',
        'retornoFinanceiroEmendasDTO'                             => 'RetornoFinanceiroEmendasDTO',
        'financeiros'                                             => 'Financeiros',
        'retornoDTO'                                              => 'RetornoDTO',
        'financeiroEmendasDTO'                                    => 'FinanceiroEmendasDTO',
        'emendas'                                                 => 'Emendas',
        'financeiroDTO'                                           => 'FinanceiroDTO',
        'emendaDTO'                                               => 'EmendaDTO',
        'consultarExecucaoOrcamentaria'                           => 'ConsultarExecucaoOrcamentaria',
        'filtroExecucaoOrcamentariaDTO'                           => 'FiltroExecucaoOrcamentariaDTO',
        'acoes'                                                   => 'Acoes',
        'acompanhamentosPO'                                       => 'AcompanhamentosPO',
        'categoriasEconomicas'                                    => 'CategoriasEconomicas',
        'detalhesAcompanhamentoPO'                                => 'DetalhesAcompanhamentoPO',
        'elementosDespesa'                                        => 'ElementosDespesa',
        'esferas'                                                 => 'Esferas',
        'fontes'                                                  => 'Fontes',
        'funcoes'                                                 => 'Funcoes',
        'gruposNatureza'                                          => 'GruposNatureza',
        'identificadoresAcompanhamentoPO'                         => 'IdentificadoresAcompanhamentoPO',
        'idocs'                                                   => 'Idocs',
        'idusos'                                                  => 'Idusos',
        'localizadores'                                           => 'Localizadores',
        'modalidadesAplicacao'                                    => 'ModalidadesAplicacao',
        'naturezasDespesa'                                        => 'NaturezasDespesa',
        'planosInternos'                                          => 'PlanosInternos',
        'planosOrcamentarios'                                     => 'PlanosOrcamentarios',
        'programas'                                               => 'Programas',
        'resultadosPrimariosAtuais'                               => 'ResultadosPrimariosAtuais',
        'resultadosPrimariosLei'                                  => 'ResultadosPrimariosLei',
        'subfuncoes'                                              => 'Subfuncoes',
        'tematicasPO'                                             => 'TematicasPO',
        'tiposCredito'                                            => 'TiposCredito',
        'tiposApropriacaoPO'                                      => 'TiposApropriacaoPO',
        'unidadesOrcamentarias'                                   => 'UnidadesOrcamentarias',
        'unidadesGestorasResponsaveis'                            => 'UnidadesGestorasResponsaveis',
        'selecaoRetornoExecucaoOrcamentariaDTO'                   => 'SelecaoRetornoExecucaoOrcamentariaDTO',
        'paginacaoDTO'                                            => 'PaginacaoDTO',
        'consultarExecucaoOrcamentariaResponse'                   => 'ConsultarExecucaoOrcamentariaResponse',
        'retornoExecucaoOrcamentariaDTO'                          => 'RetornoExecucaoOrcamentariaDTO',
        'execucoesOrcamentarias'                                  => 'ExecucoesOrcamentarias',
        'execucaoOrcamentariaDTO'                                 => 'ExecucaoOrcamentariaDTO',
        'consultarExecucaoOrcamentariaMensal'                     => 'ConsultarExecucaoOrcamentariaMensal',
        'consultarExecucaoOrcamentariaMensalResponse'             => 'ConsultarExecucaoOrcamentariaMensalResponse',
        'consultarExecucaoOrcamentariaEstataisMensal'             => 'ConsultarExecucaoOrcamentariaEstataisMensal',
        'parametrosWebExecucaoOrcamentariaDTO'                    => 'ParametrosWebExecucaoOrcamentariaDTO',
        'consultarExecucaoOrcamentariaEstataisMensalResponse'     => 'ConsultarExecucaoOrcamentariaEstataisMensalResponse',
        'retornoExecucaoOrcamentariaMensalDestDTO'                => 'RetornoExecucaoOrcamentariaMensalDestDTO',
        'execucaoOrcamentariaMensalDestDTO'                       => 'ExecucaoOrcamentariaMensalDestDTO',
        'consultarAcompanhamentoFisicoFinanceiro'                 => 'ConsultarAcompanhamentoFisicoFinanceiro',
        'consultarAcompanhamentoFisicoFinanceiroResponse'         => 'ConsultarAcompanhamentoFisicoFinanceiroResponse',
        'retornoAcompanhamentoFisicoFinanceiroDTO'                => 'RetornoAcompanhamentoFisicoFinanceiroDTO',
        'acompanhamentoFisicoFinanceiroDTO'                       => 'AcompanhamentoFisicoFinanceiroDTO',
        'consultarNaturezasDisponiveisParaProposta'               => 'ConsultarNaturezasDisponiveisParaProposta',
        'filtroNaturezaDespesaDTO'                                => 'FiltroNaturezaDespesaDTO',
        'consultarNaturezasDisponiveisParaPropostaResponse'       => 'ConsultarNaturezasDisponiveisParaPropostaResponse',
        'retornoNaturezasDTO'                                     => 'RetornoNaturezasDTO',
        'naturezaDespesaDTO'                                      => 'NaturezaDespesaDTO',
        'obterProgramacaoCompletaQuantitativo'                    => 'ObterProgramacaoCompletaQuantitativo',
        'obterProgramacaoCompletaQuantitativoResponse'            => 'ObterProgramacaoCompletaQuantitativoResponse',
        'retornoPropostasDTO'                                     => 'RetornoPropostasDTO',
        'propostaDTO'                                             => 'PropostaDTO',
        'metaPlanoOrcamentarioDTO'                                => 'MetaPlanoOrcamentarioDTO',
        'receitaDTO'                                              => 'ReceitaDTO',
        'obterInformacaoCaptacaoPLOA'                             => 'ObterInformacaoCaptacaoPLOA',
        'parametroInformacaoCaptacaoPLOA'                         => 'ParametroInformacaoCaptacaoPLOA',
        'obterInformacaoCaptacaoPLOAResponse'                     => 'ObterInformacaoCaptacaoPLOAResponse',
        'retornoInformacaoCaptacaoPLOADTO'                        => 'RetornoInformacaoCaptacaoPLOADTO',
        'registros'                                               => 'Registros',
        'informacaoCaptacaoPLOADTO'                               => 'InformacaoCaptacaoPLOADTO',
        'obterAcoesDisponiveisAcompanhamentoOrcamentario'         => 'ObterAcoesDisponiveisAcompanhamentoOrcamentario',
        'obterAcoesDisponiveisAcompanhamentoOrcamentarioResponse' => 'ObterAcoesDisponiveisAcompanhamentoOrcamentarioResponse',
        'retornoAcoesDTO'                                         => 'RetornoAcoesDTO',
        'acaoDTO'                                                 => 'AcaoDTO',
        'obterDatasCargaSIAFI'                                    => 'ObterDatasCargaSIAFI',
        'obterDatasCargaSIAFIResponse'                            => 'ObterDatasCargaSIAFIResponse',
        'retornoInformacaoCargaSiafiDTO'                          => 'RetornoInformacaoCargaSiafiDTO',
        'informacoesCargaSiafi'                                   => 'InformacoesCargaSiafi',
        'informacaoCargaSiafiDTO'                                 => 'InformacaoCargaSiafiDTO',
        'cadastrarProposta'                                       => 'CadastrarProposta',
        'cadastrarPropostaResponse'                               => 'CadastrarPropostaResponse',
        'consultarProposta'                                       => 'ConsultarProposta',
        'consultarPropostaResponse'                               => 'ConsultarPropostaResponse',
        'excluirProposta'                                         => 'ExcluirProposta',
        'excluirPropostaResponse'                                 => 'ExcluirPropostaResponse',
        'obterTabelasApoioQuantitativo'                           => 'ObterTabelasApoioQuantitativo',
        'obterTabelasApoioQuantitativoResponse'                   => 'ObterTabelasApoioQuantitativoResponse',
        'retornoApoioQuantitativoDTO'                             => 'RetornoApoioQuantitativoDTO',
        'resultadosPrimarios'                                     => 'ResultadosPrimarios',
        'naturezas'                                               => 'Naturezas',
        'idOcDTO'                                                 => 'IdOcDTO',
        'idUsoDTO'                                                => 'IdUsoDTO',
        'fonteDTO'                                                => 'FonteDTO',
        'resultadoPrimarioDTO'                                    => 'ResultadoPrimarioDTO',
        'cadastrarAcompanhamentoOrcamentario'                     => 'CadastrarAcompanhamentoOrcamentario',
        'acompanhamentoOrcamentarioAcaoDTO'                       => 'AcompanhamentoOrcamentarioAcaoDTO',
        'acompanhamentosLocalizadores'                            => 'AcompanhamentosLocalizadores',
        'acompanhamentoOrcamentarioLocalizadorDTO'                => 'AcompanhamentoOrcamentarioLocalizadorDTO',
        'analisesLocalizador'                                     => 'AnalisesLocalizador',
        'comentariosRegionalizacao'                               => 'ComentariosRegionalizacao',
        'acompanhamentosPlanoOrcamentario'                        => 'AcompanhamentosPlanoOrcamentario',
        'analiseAcompanhamentoOrcamentarioDTO'                    => 'AnaliseAcompanhamentoOrcamentarioDTO',
        'acompanhamentoPlanoOrcamentarioDTO'                      => 'AcompanhamentoPlanoOrcamentarioDTO',
        'analisesPlanoOrcamentario'                               => 'AnalisesPlanoOrcamentario',
        'cadastrarAcompanhamentoOrcamentarioResponse'             => 'CadastrarAcompanhamentoOrcamentarioResponse',
        'retornoAcompanhamentoOrcamentarioDTO'                    => 'RetornoAcompanhamentoOrcamentarioDTO',
        'acompanhamentosAcoes'                                    => 'AcompanhamentosAcoes',
        'alertas'                                                 => 'Alertas',
        'pendencias'                                              => 'Pendencias',
        'consultarAcompanhamentoOrcamentario'                     => 'ConsultarAcompanhamentoOrcamentario',
        'filtroFuncionalProgramaticaDTO'                          => 'FiltroFuncionalProgramaticaDTO',
        'consultarAcompanhamentoOrcamentarioResponse'             => 'ConsultarAcompanhamentoOrcamentarioResponse',
    ];

    public static function getComponentesSelecaoRetornoExecucaoOrcamentariaDTO ()
    {
        return array_keys(get_class_vars('SelecaoRetornoExecucaoOrcamentariaDTO'));
    }

    public static function getClassmap()
    {
        return self::$classmap;
    }
}

class ConsultarEmendasLocalizador
{
    public $credencial; // -- CredencialDTO
    public $localizador; // -- LocalizadorDTO
}

class ConsultarEmendasLocalizadorResponse
{
    public $return; // -- RetornoFinanceiroEmendasDTO
}

class RetornoFinanceiroEmendasDTO
{
    public $financeiros; // -- Financeiros
}

class Financeiros
{
    public $financeiroEmendas; // -- FinanceiroEmendasDTO
}

class FinanceiroEmendasDTO
{
    public $emendas; // -- Emendas
}

class Emendas
{
    public $emenda; // -- EmendaDTO
}

class FinanceiroDTO
{
    public $codigoPlanoOrcamentario; // -- string
    public $expansaoConcedida; // -- Long
    public $expansaoSolicitada; // -- Long
    public $fonte; // -- string
    public $idOC; // -- string
    public $idUso; // -- string
    public $identificadorPlanoOrcamentario; // -- int
    public $naturezaDespesa; // -- string
    public $resultadoPrimarioAtual; // -- string
    public $resultadoPrimarioLei; // -- string
    public $valor; // -- Long
}

class EmendaDTO
{
    public $exercicio; // -- int
    public $codAutor; // -- int
    public $decisaoParecerId; // -- int
    public $emendaNumero; // -- int
    public $emenda; // -- int
}

class ConsultarExecucaoOrcamentaria
{
    public $credencial; // -- CredencialDTO
    public $filtro; // -- FiltroExecucaoOrcamentariaDTO
    public $selecaoRetorno; // -- SelecaoRetornoExecucaoOrcamentariaDTO
    public $paginacao; // -- PaginacaoDTO
}

class FiltroExecucaoOrcamentariaDTO
{
    public $acoes; // -- Acoes
    public $acompanhamentosPO; // -- AcompanhamentosPO
    public $anoExercicio; // -- int
    public $anoReferencia; // -- int
    public $categoriasEconomicas; // -- CategoriasEconomicas
    public $detalhesAcompanhamentoPO; // -- DetalhesAcompanhamentoPO
    public $elementosDespesa; // -- ElementosDespesa
    public $esferas; // -- Esferas
    public $estatalDependente; // -- boolean
    public $fontes; // -- Fontes
    public $funcoes; // -- Funcoes
    public $gruposNatureza; // -- GruposNatureza
    public $identificadoresAcompanhamentoPO; // -- IdentificadoresAcompanhamentoPO
    public $idocs; // -- Idocs
    public $idusos; // -- Idusos
    public $localizadores; // -- Localizadores
    public $modalidadesAplicacao; // -- ModalidadesAplicacao
    public $naturezasDespesa; // -- NaturezasDespesa
    public $planosInternos; // -- PlanosInternos
    public $planosOrcamentarios; // -- PlanosOrcamentarios
    public $programas; // -- Programas
    public $resultadosPrimariosAtuais; // -- ResultadosPrimariosAtuais
    public $resultadosPrimariosLei; // -- ResultadosPrimariosLei
    public $subfuncoes; // -- Subfuncoes
    public $tematicasPO; // -- TematicasPO
    public $tiposCredito; // -- TiposCredito
    public $tiposApropriacaoPO; // -- TiposApropriacaoPO
    public $unidadesOrcamentarias; // -- UnidadesOrcamentarias
    public $unidadesGestorasResponsaveis; // -- UnidadesGestorasResponsaveis
}

class AcompanhamentosPO
{
    public $acompanhamentoPO; // -- string
}

class CategoriasEconomicas
{
    public $categoriaEconomicas; // -- string
}

class DetalhesAcompanhamentoPO
{
    public $detalheAcompanhamentoPO; // -- string
}

class ElementosDespesa
{
    public $elementoDespesa; // -- string
}

class Esferas
{
    public $esfera; // -- string
}

class Fontes
{
    public $fonte; // -- string
}

class Funcoes
{
    public $funcao; // -- string
}

class GruposNatureza
{
    public $grupoNatureza; // -- string
}

class IdentificadoresAcompanhamentoPO
{
    public $identificadorAcompanhamentoPO; // -- string
}

class Idocs
{
    public $idoc; // -- string
}

class Idusos
{
    public $iduso; // -- string
}

class ModalidadesAplicacao
{
    public $modalidadeAplicacao; // -- string
}

class NaturezasDespesa
{
    public $naturezaDespesa; // -- string
}

class PlanosInternos
{
    public $planoInterno; // -- string
}

class PlanosOrcamentarios
{
    public $planoOrcamentario; // -- string
}

class Programas
{
    public $programa; // -- string
}

class ResultadosPrimariosAtuais
{
    public $resultadoPrimarioAtual; // -- string
}

class ResultadosPrimariosLei
{
    public $resultadoPrimarioLei; // -- string
}

class Subfuncoes
{
    public $subfuncao; // -- string
}

class TematicasPO
{
    public $tematicaPO; // -- string
}

class TiposCredito
{
    public $tipoCredito; // -- string
}

class TiposApropriacaoPO
{
    public $tipoApropriacaoPO; // -- string
}

class UnidadesOrcamentarias
{
    public $unidadeOrcamentaria; // -- string
}

class UnidadesGestorasResponsaveis
{
    public $unidadeGestoraResponsavel; // -- string
}

class SelecaoRetornoExecucaoOrcamentariaDTO
{
    public $acao; // -- boolean
    public $acompanhamentoPO; // -- boolean
    public $anoExercicio; // -- boolean
    public $anoReferencia; // -- boolean
    public $autorizado; // -- boolean
    public $bloqueadoRemanejamento; // -- boolean
    public $bloqueadoSOF; // -- boolean
    public $categoriaEconomica; // -- boolean
    public $creditoContidoSOF; // -- boolean
    public $detalheAcompanhamentoPO; // -- boolean
    public $disponivel; // -- boolean
    public $dotAtual; // -- boolean
    public $dotInicialSiafi; // -- boolean
    public $dotacaoAntecipada; // -- boolean
    public $dotacaoInicial; // -- boolean
    public $elementoDespesa; // -- boolean
    public $empLiquidado; // -- boolean
    public $empenhadoALiquidar; // -- boolean
    public $esfera; // -- boolean
    public $estatalDependente; // -- boolean
    public $executadoPorInscricaoDeRAP; // -- boolean
    public $fonte; // -- boolean
    public $funcao; // -- boolean
    public $grupoNaturezaDespesa; // -- boolean
    public $identificadorAcompanhamentoPO; // -- boolean
    public $idoc; // -- boolean
    public $iduso; // -- boolean
    public $indisponivel; // -- boolean
    public $localizador; // -- boolean
    public $modalidadeAplicacao; // -- boolean
    public $natureza; // -- boolean
    public $numeroemenda; // -- boolean
    public $numeroptres; // -- boolean
    public $origem; // -- boolean
    public $pago; // -- boolean
    public $planoInterno; // -- boolean
    public $planoOrcamentario; // -- boolean
    public $programa; // -- boolean
    public $projetoLei; // -- boolean
    public $rapAPagarNaoProcessado; // -- boolean
    public $rapAPagarProcessado; // -- boolean
    public $rapCanceladosNaoProcessados; // -- boolean
    public $rapCanceladosProcessados; // -- boolean
    public $rapExerciciosAnteriores; // -- boolean
    public $rapInscritoNaoProcessado; // -- boolean
    public $rapInscritoProcessado; // -- boolean
    public $rapNaoProcessadoALiquidar; // -- boolean
    public $rapNaoProcessadoBloqueado; // -- boolean
    public $rapNaoProcessadoLiquidadoAPagar; // -- boolean
    public $rapPagoNaoProcessado; // -- boolean
    public $rapPagoProcessado; // -- boolean
    public $resultadoPrimarioAtual; // -- boolean
    public $resultadoPrimarioLei; // -- boolean
    public $subElementoDespesa; // -- boolean
    public $subFuncao; // -- boolean
    public $tematicaPO; // -- boolean
    public $tipoApropriacaoPO; // -- boolean
    public $tipoCredito; // -- boolean
    public $unidadeGestoraResponsavel; // -- boolean
    public $unidadeOrcamentaria; // -- boolean
}

class PaginacaoDTO
{
    public $pagina; // -- int
    public $registrosPorPagina; // -- int
}

class ConsultarExecucaoOrcamentariaResponse
{
    public $return; // -- RetornoExecucaoOrcamentariaDTO
}

class RetornoExecucaoOrcamentariaDTO
{
    public $dataHoraUltimaCarga; // -- dateTime
    public $execucoesOrcamentarias; // -- ExecucoesOrcamentarias
    public $paginacao; // -- PaginacaoDTO
}

class ExecucoesOrcamentarias
{
    public $execucaoOrcamentaria; // -- ExecucaoOrcamentariaDTO
}

class ExecucaoOrcamentariaDTO
{
    public $acao; // -- string
    public $acompanhamento; // -- string
    public $acompanhamentoPO; // -- string
    public $anoExercicio; // -- int
    public $anoReferencia; // -- int
    public $autorizado; // -- decimal
    public $bloqueadoRemanejamento; // -- decimal
    public $bloqueadoSOF; // -- decimal
    public $categoriaEconomica; // -- string
    public $credito; // -- string
    public $creditoContidoSOF; // -- decimal
    public $detalheAcompanhamento; // -- string
    public $detalheAcompanhamentoPO; // -- string
    public $disponivel; // -- decimal
    public $dotAntecipada; // -- decimal
    public $dotAtual; // -- decimal
    public $dotInicial; // -- decimal
    public $dotInicialSiafi; // -- decimal
    public $dotacaoAntecipada; // -- decimal
    public $dotacaoInicial; // -- decimal
    public $dotacaoOriginal; // -- decimal
    public $elementoDespesa; // -- string
    public $empALiquidar; // -- decimal
    public $empLiqInscrRapNp; // -- string
    public $empLiquidado; // -- decimal
    public $empenhadoALiquidar; // -- decimal
    public $esfera; // -- string
    public $executadoPorInscricaoDeRAP; // -- decimal
    public $fonte; // -- string
    public $funcao; // -- string
    public $grupoNaturezaDespesa; // -- string
    public $idOc; // -- string
    public $idUso; // -- string
    public $identificadorAcompanhamento; // -- string
    public $identificadorAcompanhamentoPO; // -- string
    public $indisponivel; // -- decimal
    public $localizador; // -- string
    public $mes; // -- int
    public $modalidadeAplicacao; // -- string
    public $natureza; // -- string
    public $numeroemenda; // -- string
    public $numeroptres; // -- string
    public $origem; // -- string
    public $pago; // -- decimal
    public $planoInterno; // -- string
    public $planoOrcamentario; // -- string
    public $programa; // -- string
    public $projetoLei; // -- decimal
    public $rapAPagarNaoProcessado; // -- decimal
    public $rapAPagarProcessado; // -- decimal
    public $rapCanceladosNaoProcessados; // -- decimal
    public $rapCanceladosProcessados; // -- decimal
    public $rapExerciciosAnteriores; // -- decimal
    public $rapInscritoNaoProcessado; // -- decimal
    public $rapInscritoProcessado; // -- decimal
    public $rapNaoProcessadoALiquidar; // -- decimal
    public $rapNaoProcessadoBloqueado; // -- decimal
    public $rapNaoProcessadoLiquidadoAPagar; // -- decimal
    public $rapPagoNaoProcessado; // -- decimal
    public $rapPagoProcessado; // -- decimal
    public $resultadoPrimarioAtual; // -- string
    public $resultadoPrimarioLei; // -- string
    public $rpAtual; // -- string
    public $rpLei; // -- string
    public $subElementoDespesa; // -- string
    public $subFuncao; // -- string
    public $tematica; // -- string
    public $tematicaPO; // -- string
    public $tipoApropriacao; // -- string
    public $tipoApropriacaoPO; // -- string
    public $tipoCredito; // -- string
    public $unidadeGestoraResponsavel; // -- string
    public $unidadeOrcamentaria; // -- string
}

class ConsultarExecucaoOrcamentariaMensal
{
    public $credencial; // -- CredencialDTO
    public $filtro; // -- FiltroExecucaoOrcamentariaDTO
    public $selecaoRetorno; // -- SelecaoRetornoExecucaoOrcamentariaDTO
    public $mes; // -- int
    public $paginacao; // -- PaginacaoDTO
}

class ConsultarExecucaoOrcamentariaMensalResponse
{
    public $return; // -- RetornoExecucaoOrcamentariaDTO
}

class ConsultarExecucaoOrcamentariaEstataisMensal
{
    public $credencial; // -- CredencialDTO
    public $parametros; // -- ParametrosWebExecucaoOrcamentariaDTO
    public $paginacao; // -- PaginacaoDTO
}

class ParametrosWebExecucaoOrcamentariaDTO
{
    public $acao; // -- string
    public $esfera; // -- string
    public $exercicio; // -- int
    public $funcao; // -- string
    public $localizador; // -- string
    public $orgao; // -- string
    public $programa; // -- string
    public $subFuncao; // -- string
    public $unidadeOrcamentaria; // -- string
}

class ConsultarExecucaoOrcamentariaEstataisMensalResponse
{
    public $return; // -- RetornoExecucaoOrcamentariaMensalDestDTO
}

class RetornoExecucaoOrcamentariaMensalDestDTO
{
    public $paginacao; // -- PaginacaoDTO
    public $registros; // -- ExecucaoOrcamentariaMensalDestDTO
}

class ExecucaoOrcamentariaMensalDestDTO
{
    public $acao; // -- string
    public $descricaoFuncao; // -- string
    public $descricaoSubfuncao; // -- string
    public $esfera; // -- string
    public $estatalDependente; // -- boolean
    public $estatalIndependente; // -- boolean
    public $exercicio; // -- int
    public $funcao; // -- string
    public $lei; // -- decimal
    public $leiMaisCreditos; // -- decimal
    public $localizador; // -- string
    public $ppipac; // -- boolean
    public $programa; // -- string
    public $realizadaAbril; // -- decimal
    public $realizadaAgosto; // -- decimal
    public $realizadaDezembro; // -- decimal
    public $realizadaFevereiro; // -- decimal
    public $realizadaJaneiro; // -- decimal
    public $realizadaJulho; // -- decimal
    public $realizadaJunho; // -- decimal
    public $realizadaMaio; // -- decimal
    public $realizadaMarco; // -- decimal
    public $realizadaNovembro; // -- decimal
    public $realizadaOutubro; // -- decimal
    public $realizadaSetembro; // -- decimal
    public $regiao; // -- string
    public $subfuncao; // -- string
    public $tituloAcao; // -- string
    public $tituloLocalizador; // -- string
    public $tituloPrograma; // -- string
    public $uf; // -- string
    public $unidadeOrcamentaria; // -- string
}

class ConsultarAcompanhamentoFisicoFinanceiro
{
    public $credencial; // -- CredencialDTO
    public $exercicio; // -- int
    public $periodo; // -- int
    public $momentoId; // -- int
    public $tipoCaptacao; // -- string
    public $paginacao; // -- PaginacaoDTO
}

class ConsultarAcompanhamentoFisicoFinanceiroResponse
{
    public $return; // -- RetornoAcompanhamentoFisicoFinanceiroDTO
}

class RetornoAcompanhamentoFisicoFinanceiroDTO
{
    public $dataHoraUltimaCargaSiafi; // -- dateTime
    public $paginacao; // -- PaginacaoDTO
    public $quantidadeMetaAtual; // -- decimal
    public $quantidadeMetaLOA; // -- decimal
    public $quantidadeMetaLOAPO; // -- decimal
    public $realizadoLOA; // -- decimal
    public $realizadoPO; // -- decimal
    public $realizadoRAP; // -- decimal
    public $registros; // -- AcompanhamentoFisicoFinanceiroDTO
    public $reprogramadoFisico; // -- decimal
}

class AcompanhamentoFisicoFinanceiroDTO
{
    public $acao; // -- string
    public $codigoPO; // -- string
    public $descricaoFuncao; // -- string
    public $descricaoLocalizador; // -- string
    public $descricaoOrgao; // -- string
    public $descricaoProduto; // -- string
    public $descricaoProdutoPO; // -- string
    public $descricaoSiorg; // -- string
    public $descricaoSubFuncao; // -- string
    public $descricaoUnidadeMedida; // -- string
    public $descricaoUnidadeMedidaPO; // -- string
    public $descricaoUo; // -- string
    public $dotacaoAtual; // -- decimal
    public $dotacaoAtualPO; // -- decimal
    public $dotacaoInicial; // -- decimal
    public $dotacaoInicialPO; // -- decimal
    public $esfera; // -- string
    public $exercicio; // -- int
    public $funcao; // -- string
    public $liquidado; // -- decimal
    public $liquidadoPO; // -- decimal
    public $liquidadoRAP; // -- decimal
    public $liquidadoRAPPO; // -- decimal
    public $localizador; // -- string
    public $momento; // -- string
    public $momentoId; // -- int
    public $orgao; // -- string
    public $orgaoSiorg; // -- string
    public $pago; // -- decimal
    public $pagoPO; // -- decimal
    public $periodo; // -- string
    public $produto; // -- int
    public $produtoPO; // -- int
    public $programa; // -- string
    public $quantidadeMetaAtual; // -- decimal
    public $quantidadeMetaAtualPO; // -- decimal
    public $quantidadeMetaLOA; // -- decimal
    public $quantidadeMetaLOAPO; // -- decimal
    public $realizadoLOA; // -- decimal
    public $realizadoPO; // -- decimal
    public $realizadoRAP; // -- decimal
    public $reprogramadoFinanceiro; // -- decimal
    public $reprogramadoFisico; // -- decimal
    public $subfuncao; // -- string
    public $tipoCaptacao; // -- string
    public $tituloAcao; // -- string
    public $tituloPO; // -- string
    public $tituloPrograma; // -- string
    public $unidadeMedida; // -- string
    public $unidadeMedidaPO; // -- string
    public $uo; // -- string
}

class ConsultarNaturezasDisponiveisParaProposta
{
    public $credencial; // -- CredencialDTO
    public $filtro; // -- FiltroNaturezaDespesaDTO
}

class FiltroNaturezaDespesaDTO
{
    public $codigoAcao; // -- string
    public $codigoDetalhamento; // -- string
    public $codigoOrgao; // -- string
    public $exercicio; // -- int
    public $momentoId; // -- int
}

class ConsultarNaturezasDisponiveisParaPropostaResponse
{
    public $return; // -- RetornoNaturezasDTO
}

class RetornoNaturezasDTO
{
    public $registros; // -- NaturezaDespesaDTO
}

class NaturezaDespesaDTO
{
    public $codigoNatureza; // -- string
    public $elementoDescricao; // -- string
    public $elementoDescricaoAbreviada; // -- string
    public $exercicio; // -- int
    public $snAtiva; // -- boolean
    public $snValorizavel; // -- boolean
    public $subElementoDescricao; // -- string
    public $subElementoDescricaoAbreviada; // -- string
}

class ObterProgramacaoCompletaQuantitativo
{
    public $credencial; // -- CredencialDTO
    public $exercicio; // -- int
    public $codigoMomento; // -- int
    public $dataHoraReferencia; // -- dateTime
    public $paginacao; // -- PaginacaoDTO
}

class ObterProgramacaoCompletaQuantitativoResponse
{
    public $return; // -- RetornoPropostasDTO
}

class RetornoPropostasDTO
{
    public $numeroRegistros; // -- int
    public $proposta; // -- PropostaDTO
    public $valorTotal; // -- Long
}

class PropostaDTO
{
    public $codigoAcao; // -- string
    public $codigoEsfera; // -- string
    public $codigoFuncao; // -- string
    public $codigoLocalizador; // -- string
    public $codigoMomento; // -- int
    public $codigoOrgao; // -- string
    public $codigoPrograma; // -- string
    public $codigoSubFuncao; // -- string
    public $codigoTipoDetalhamento; // -- string
    public $codigoTipoInclusaoAcao; // -- int
    public $codigoTipoInclusaoLocalizador; // -- int
    public $exercicio; // -- int
    public $expansaoFisicaConcedida; // -- Long
    public $expansaoFisicaSolicitada; // -- Long
    public $financeiros; // -- FinanceiroDTO
    public $identificadorUnicoAcao; // -- int
    public $justificativa; // -- string
    public $justificativaExpansaoConcedida; // -- string
    public $justificativaExpansaoSolicitada; // -- string
    public $metaPlanoOrcamentario; // -- MetaPlanoOrcamentarioDTO
    public $quantidadeFisico; // -- Long
    public $receitas; // -- ReceitaDTO
    public $snAtual; // -- boolean
    public $valorFisico; // -- Long
}

class MetaPlanoOrcamentarioDTO
{
    public $expansaoFisicaConcedida; // -- Long
    public $expansaoFisicaSolicitada; // -- Long
    public $identificadorUnicoPlanoOrcamentario; // -- int
    public $quantidadeFisico; // -- Long
}

class ReceitaDTO
{
    public $naturezaReceita; // -- string
    public $valor; // -- Long
}

class ObterInformacaoCaptacaoPLOA
{
    public $credencial; // -- CredencialDTO
    public $parametro; // -- ParametroInformacaoCaptacaoPLOA
}

class ParametroInformacaoCaptacaoPLOA
{
    public $exercicio; // -- int
    public $codigoMomento; // -- int
    public $codigoTipoDetalhamento; // -- string
    public $codigoOrgao; // -- string
    public $codigoUnidadeOrcamentaria; // -- string
    public $captados; // -- boolean
    public $captaveis; // -- boolean
}

class ObterInformacaoCaptacaoPLOAResponse
{
    public $return; // -- RetornoInformacaoCaptacaoPLOADTO
}

class RetornoInformacaoCaptacaoPLOADTO
{
    public $registros; // -- Registros
}

class Registros
{
    public $registro; // -- InformacaoCaptacaoPLOADTO
}

class InformacaoCaptacaoPLOADTO
{
    public $codigoMomentoAcao; // -- int
    public $codigoMomentoJanelaAtual; // -- int
    public $codigoMomentoLocalizador; // -- int
    public $codigoMomentoPropostaAtual; // -- int
    public $codigoTipoDetalhamento; // -- string
    public $exercicio; // -- int
    public $funcional; // -- string
    public $identificadorUnicoAcao; // -- int
    public $identificadorUnicoLocalizador; // -- int
    public $podeCaptar; // -- boolean
    public $porQueNaoPodeCaptar; // -- string
    public $propostaValida; // -- boolean
    public $temJanela; // -- boolean
    public $temProposta; // -- boolean
}

class ObterAcoesDisponiveisAcompanhamentoOrcamentario
{
    public $credencial; // -- CredencialDTO
    public $exercicio; // -- int
    public $periodo; // -- int
    public $snAcompanhamentoOpcionalAcao; // -- boolean
    public $snAcompanhamentoOpcionalLocalizador; // -- boolean
}

class ObterAcoesDisponiveisAcompanhamentoOrcamentarioResponse
{
    public $return; // -- RetornoAcoesDTO
}

class ObterDatasCargaSIAFI
{
    public $credencial; // -- CredencialDTO
}

class ObterDatasCargaSIAFIResponse
{
    public $return; // -- RetornoInformacaoCargaSiafiDTO
}

class RetornoInformacaoCargaSiafiDTO
{
    public $informacoesCargaSiafi; // -- InformacoesCargaSiafi
}

class InformacoesCargaSiafi
{
    public $informacaoCargaSiafi; // -- InformacaoCargaSiafiDTO
}

class InformacaoCargaSiafiDTO
{
    public $tipo; // -- string
    public $dataCompetencia; // -- dateTime
    public $ultimaCarga; // -- dateTime
    public $ultimoMesFechado; // -- string
    public $dataFechamentoUltimoMes; // -- dateTime
}

class CadastrarProposta
{
    public $credencial; // -- CredencialDTO
    public $proposta; // -- PropostaDTO
}

class CadastrarPropostaResponse
{
    public $return; // -- RetornoPropostasDTO
}

class ConsultarProposta
{
    public $credencial; // -- CredencialDTO
    public $proposta; // -- PropostaDTO
}

class ConsultarPropostaResponse
{
    public $return; // -- RetornoPropostasDTO
}

class ExcluirProposta
{
    public $credencial; // -- CredencialDTO
    public $proposta; // -- PropostaDTO
}

class ExcluirPropostaResponse
{
    public $return; // -- RetornoPropostasDTO
}

class ObterTabelasApoioQuantitativo
{
    public $credencial; // -- CredencialDTO
    public $exercicio; // -- int
    public $retornarNaturezas; // -- boolean
    public $retornarIdOcs; // -- boolean
    public $retornarIdUsos; // -- boolean
    public $retornarFontes; // -- boolean
    public $retornarRPs; // -- boolean
    public $dataHoraReferencia; // -- dateTime
}

class ObterTabelasApoioQuantitativoResponse
{
    public $return; // -- RetornoApoioQuantitativoDTO
}

class RetornoApoioQuantitativoDTO
{
    public $idocs; // -- Idocs
    public $idusos; // -- Idusos
    public $fontes; // -- Fontes
    public $resultadosPrimarios; // -- ResultadosPrimarios
    public $naturezas; // -- Naturezas
}

class ResultadosPrimarios
{
    public $resultadoPrimario; // -- ResultadoPrimarioDTO
}

class Naturezas
{
    public $natureza; // -- NaturezaDespesaDTO
}

class IdOcDTO
{
    public $codigoIdOc; // -- string
    public $descricao; // -- string
    public $exercicio; // -- int
    public $snAtivo; // -- boolean
}

class IdUsoDTO
{
    public $codigoIdUso; // -- string
    public $descricao; // -- string
    public $exercicio; // -- int
    public $snAtivo; // -- boolean
}

class FonteDTO
{
    public $codigoFonte; // -- string
    public $descricao; // -- string
    public $exercicio; // -- int
    public $snAtivo; // -- boolean
}

class ResultadoPrimarioDTO
{
    public $codigoResultadoPrimario; // -- string
    public $descricao; // -- string
    public $exercicio; // -- int
}

class CadastrarAcompanhamentoOrcamentario
{
    public $credencial; // -- CredencialDTO
    public $acompanhamentoAcao; // -- AcompanhamentoOrcamentarioAcaoDTO
}

class AcompanhamentoOrcamentarioAcaoDTO
{
    public $periodoOrdem; // -- int
    public $exercicio; // -- int
    public $codigoMomento; // -- int
    public $esfera; // -- string
    public $unidadeOrcamentaria; // -- string
    public $funcao; // -- string
    public $subFuncao; // -- string
    public $programa; // -- string
    public $acao; // -- string
    public $codigoTipoInclusaoAcao; // -- int
    public $snPendencia; // -- boolean
    public $dataHoraAlteracao; // -- dateTime
    public $acompanhamentosLocalizadores; // -- AcompanhamentosLocalizadores
}

class AcompanhamentosLocalizadores
{
    public $acompanhamentoLocalizador; // -- AcompanhamentoOrcamentarioLocalizadorDTO
}

class AcompanhamentoOrcamentarioLocalizadorDTO
{
    public $localizador; // -- string
    public $codigoTipoInclusaoLocalizador; // -- int
    public $meta; // -- Long
    public $reprogramado; // -- Long
    public $realizadoLOA; // -- Long
    public $dataApuracaoLOA; // -- dateTime
    public $dotacaoAtual; // -- Long
    public $dotacaoInicial; // -- Long
    public $limite; // -- Long
    public $empenhado; // -- double
    public $liquidado; // -- double
    public $pago; // -- double
    public $realizadoRAP; // -- Long
    public $dataApuracaoRAP; // -- dateTime
    public $rapInscritoLiquido; // -- double
    public $rapLiquidadoAPagar; // -- double
    public $rapPago; // -- double
    public $rapLiquidadoEfetivo; // -- double
    public $justificativa; // -- string
    public $analisesLocalizador; // -- AnalisesLocalizador
    public $comentariosRegionalizacao; // -- ComentariosRegionalizacao
    public $acompanhamentosPlanoOrcamentario; // -- AcompanhamentosPlanoOrcamentario
}

class AnalisesLocalizador
{
    public $analiseLocalizador; // -- AnaliseAcompanhamentoOrcamentarioDTO
}

class ComentariosRegionalizacao
{
    public $comentarioRegionalizacao; // -- AnaliseAcompanhamentoOrcamentarioDTO
}

class AcompanhamentosPlanoOrcamentario
{
    public $acompanhamentoPlanoOrcamentario; // -- AcompanhamentoPlanoOrcamentarioDTO
}

class AnaliseAcompanhamentoOrcamentarioDTO
{
    public $analise; // -- string
    public $comentarioId; // -- int
    public $nomeUsuario; // -- string
    public $periodoOrdem; // -- int
    public $ultimaModificacao; // -- dateTime
}

class AcompanhamentoPlanoOrcamentarioDTO
{
    public $planoOrcamentario; // -- string
    public $realizadoLOA; // -- Long
    public $dataApuracaoLOA; // -- dateTime
    public $analisesPlanoOrcamentario; // -- AnalisesPlanoOrcamentario
    public $descricaoProduto; // -- string
    public $descricaoUnidadeMedida; // -- string
    public $titulo; // -- string
    public $dotacaoAtual; // -- double
    public $empenhado; // -- double
    public $liquidado; // -- double
    public $pago; // -- double
    public $meta; // -- Long
}

class AnalisesPlanoOrcamentario
{
    public $analisePlanoOrcamentario; // -- AnaliseAcompanhamentoOrcamentarioDTO
}

class CadastrarAcompanhamentoOrcamentarioResponse
{
    public $return; // -- RetornoAcompanhamentoOrcamentarioDTO
}

class RetornoAcompanhamentoOrcamentarioDTO
{
    public $acompanhamentosAcoes; // -- AcompanhamentosAcoes
    public $alertas; // -- Alertas
    public $pendencias; // -- Pendencias
}

class AcompanhamentosAcoes
{
    public $acompanhamentoAcao; // -- AcompanhamentoOrcamentarioAcaoDTO
}

class Alertas
{
    public $alerta; // -- string
}

class Pendencias
{
    public $pendencia; // -- string
}

class ConsultarAcompanhamentoOrcamentario
{
    public $credencial; // -- CredencialDTO
    public $exercicio; // -- int
    public $periodoOrdem; // -- int
    public $codigoMomento; // -- int
    public $filtro; // -- FiltroFuncionalProgramaticaDTO
    public $dataHoraReferencia; // -- dateTime
}

class FiltroFuncionalProgramaticaDTO
{
    public $codigoAcao; // -- string
    public $codigoEsfera; // -- string
    public $codigoFuncao; // -- string
    public $codigoLocalizador; // -- string
    public $codigoPrograma; // -- string
    public $codigoSubFuncao; // -- string
    public $codigoTipoInclusaoAcao; // -- int
    public $codigoTipoInclusaoLocalizador; // -- int
    public $codigoUO; // -- string
    public $exercicio; // -- int
    public $tipoInclusaoIds; // -- int
}

class ConsultarAcompanhamentoOrcamentarioResponse
{
    public $return; // -- RetornoAcompanhamentoOrcamentarioDTO
}
