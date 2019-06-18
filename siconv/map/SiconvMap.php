<?php
/**
 * Created by PhpStorm.
 * User: victor
 * Date: 13/04/17
 * Time: 10:44
 */

class Ws_SiconvMap {

    public static $classmap = [
        'ObterVersaoWebService',
        'obterVersaoWebServiceResponse'=>'ObterVersaoWebServiceResponse',
        'consultaPropostasAlteradasOrgaoPeriodo'=>'ConsultaPropostasAlteradasOrgaoPeriodo',
        'propostasAlteradasExportacaoWS'=>'PropostasAlteradasExportacaoWS',
        'abstractWS'=>'AbstractWS',
        'loginWS'=>'LoginWS',
        'consultaPropostasAlteradasOrgaoPeriodoResponse'=>'ConsultaPropostasAlteradasOrgaoPeriodoResponse',
        'propostasWS'=>'PropostasWS',
        'propostaWS'=>'PropostaWS',
        'bensServicosPropostaWS'=>'BensServicosPropostaWS',
        'municipioWS'=>'MunicipioWS',
        'unidadeFederativaWS'=>'UnidadeFederativaWS',
        'naturezaAquisicaoWS'=>'NaturezaAquisicaoWS',
        'naturezaDespesaSubItemWS'=>'NaturezaDespesaSubItemWS',
        'naturezaDespesaWS'=>'NaturezaDespesaWS',
        'propostaProgramaWS'=>'PropostaProgramaWS',
        'objetoWS'=>'ObjetoWS',
        'origemRecursoPropProgramaWS'=>'OrigemRecursoPropProgramaWS',
        'cnpjBeneficiarioEspecificoWS'=>'CnpjBeneficiarioEspecificoWS',
        'cnpjProgramaEmendaWS'=>'CnpjProgramaEmendaWS',
        'programaWS'=>'ProgramaWS',
        'orgaoAdministrativoWS'=>'OrgaoAdministrativoWS',
        'responsaveisConvenioWS'=>'ResponsaveisConvenioWS',
        'membroParticipanteWS'=>'MembroParticipanteWS',
        'participantesPropostaWS'=>'ParticipantesPropostaWS',
        'alteracaoEstatuariaWS'=>'AlteracaoEstatuariaWS',
        'areaAtuacaoEntidadePrivadaWS'=>'AreaAtuacaoEntidadePrivadaWS',
        'certidaoWS'=>'CertidaoWS',
        'respostaCertidaoWS'=>'RespostaCertidaoWS',
        'cnaeWS'=>'CnaeWS',
        'declaracaoNaoDividaWS'=>'DeclaracaoNaoDividaWS',
        'dirigenteWS'=>'DirigenteWS',
        'esferaAdministrativaWS'=>'EsferaAdministrativaWS',
        'estatutoWS'=>'EstatutoWS',
        'tipoIdentificadorParticipeWS'=>'TipoIdentificadorParticipeWS',
        'modalidadeConvenioWS'=>'ModalidadeConvenioWS',
        'naturezaJuridicaWS'=>'NaturezaJuridicaWS',
        'regrasWS'=>'RegrasWS',
        'qualificacaoProponenteWS'=>'QualificacaoProponenteWS',
        'statusBensServicosPropostaWS'=>'StatusBensServicosPropostaWS',
        'contaBancariaWS'=>'ContaBancariaWS',
        'bancoWS'=>'BancoWS',
        'cronogramaDesembolsoPropostaWS'=>'CronogramaDesembolsoPropostaWS',
        'parcelaCronogramaDesembolsoWS'=>'ParcelaCronogramaDesembolsoWS',
        'parcelaMetaWS'=>'ParcelaMetaWS',
        'parcelaEtapaWS'=>'ParcelaEtapaWS',
        'tipoParticipanteWS'=>'TipoParticipanteWS',
        'cronogramaFisicoPropostaWS'=>'CronogramaFisicoPropostaWS',
        'metaCronogramaFisicoWS'=>'MetaCronogramaFisicoWS',
        'etapaCronogramaFisicoWS'=>'EtapaCronogramaFisicoWS',
        'despesaAdministrativaWS'=>'DespesaAdministrativaWS',
        'intervenientePropostaWS'=>'IntervenientePropostaWS',
        'modalidadePropostaWS'=>'ModalidadePropostaWS',
        'motivoNaoAcatamentoContratoRepasseWS'=>'MotivoNaoAcatamentoContratoRepasseWS',
        'parecerPlanoTrabalhoWS'=>'ParecerPlanoTrabalhoWS',
        'parecerPropostaConvenioWS'=>'ParecerPropostaConvenioWS',
        'repasseExercicioFuturoPropostaWS'=>'RepasseExercicioFuturoPropostaWS',
        'situacaoEnvioContratoRepasseWS'=>'SituacaoEnvioContratoRepasseWS',
        'situacaoProjetoBasicoWS'=>'SituacaoProjetoBasicoWS',
        'situacaoPropostaWS'=>'SituacaoPropostaWS',
        'tipoNaoAcatamentoContratoRepasseWS'=>'TipoNaoAcatamentoContratoRepasseWS',
        'tipoProjetoBasicoWS'=>'TipoProjetoBasicoWS',
        'transferenciaRecursosWS'=>'TransferenciaRecursosWS',
        'obConfluxoExportacaoWS'=>'ObConfluxoExportacaoWS',
        'dadosOrcamentariosWS'=>'DadosOrcamentariosWS',
        'ordemBancariaExportacaoWS'=>'OrdemBancariaExportacaoWS',
        'eventoOrdemBancariaWS'=>'EventoOrdemBancariaWS',
        'programacaoFinanceiraExportacaoWS'=>'ProgramacaoFinanceiraExportacaoWS',
        'consultaPropostasOrgao'=>'ConsultaPropostasOrgao',
        'propostaExportacaoWS'=>'PropostaExportacaoWS',
        'consultaPropostasOrgaoResponse'=>'ConsultaPropostasOrgaoResponse',
        'retornoConsultaPropostasOrgaoWS'=>'RetornoConsultaPropostasOrgaoWS',
        'propostaConsultaWS'=>'PropostaConsultaWS',
        'consultarConveniosOrgaoPorAno'=>'ConsultarConveniosOrgaoPorAno',
        'parametrosConsultarConveniosOrgaoPorAnoWS'=>'ParametrosConsultarConveniosOrgaoPorAnoWS',
        'consultarConveniosOrgaoPorAnoResponse'=>'ConsultarConveniosOrgaoPorAnoResponse',
        'convenioWS'=>'ConvenioWS',
        'notaEmpenhoWS'=>'NotaEmpenhoWS',
        'contaCorrenteWS'=>'ContaCorrenteWS',
        'cronogramaEmpenhoWS'=>'CronogramaEmpenhoWS',
        'tipoNotaEmpenhoWS'=>'TipoNotaEmpenhoWS',
        'itemEmpenhoWS'=>'ItemEmpenhoWS',
        'tipoEmpenhoWS'=>'TipoEmpenhoWS',
        'notaLancamentoWS'=>'NotaLancamentoWS',
        'eventoNotaLancamentoWS'=>'EventoNotaLancamentoWS',
        'propostaConvenioWS'=>'PropostaConvenioWS',
        'prorrogaOficioWS'=>'ProrrogaOficioWS',
        'termoAditivoWS'=>'TermoAditivoWS',
        'repasseExercicioFuturoTermoAditivoWS'=>'RepasseExercicioFuturoTermoAditivoWS',
        'tipoTaWS'=>'TipoTaWS',
        'consultarPrograma'=>'ConsultarPrograma',
        'parametrosConsultarProgramaWS'=>'ParametrosConsultarProgramaWS',
        'consultarProgramaResponse'=>'ConsultarProgramaResponse',
        'consultarPropostaPorAcaoOrcamentaria'=>'ConsultarPropostaPorAcaoOrcamentaria',
        'parametrosConsultarPropostaPorAcaoOrcamentariaWS'=>'ParametrosConsultarPropostaPorAcaoOrcamentariaWS',
        'consultarPropostaPorAcaoOrcamentariaResponse'=>'ConsultarPropostaPorAcaoOrcamentariaResponse',
        'consultarPropostaPorCNPJ'=>'ConsultarPropostaPorCNPJ',
        'parametrosConsultarPropostaPorCNPJWS'=>'ParametrosConsultarPropostaPorCNPJWS',
        'consultarPropostaPorCNPJResponse'=>'ConsultarPropostaPorCNPJResponse',
        'consultarPropostaPorEmendaParlamentar'=>'ConsultarPropostaPorEmendaParlamentar',
        'parametrosConsultarPropostaPorEmendaParlamentarWS'=>'ParametrosConsultarPropostaPorEmendaParlamentarWS',
        'consultarPropostaPorEmendaParlamentarResponse'=>'ConsultarPropostaPorEmendaParlamentarResponse',
        'consultarPropostaPorMunicipio'=>'ConsultarPropostaPorMunicipio',
        'parametrosConsultarPropostaPorMunicipioWS'=>'ParametrosConsultarPropostaPorMunicipioWS',
        'consultarPropostaPorMunicipioResponse'=>'ConsultarPropostaPorMunicipioResponse',
        'consultarPropostaPorPrograma'=>'ConsultarPropostaPorPrograma',
        'parametrosConsultarPropostaPorProgramaWS'=>'ParametrosConsultarPropostaPorProgramaWS',
        'consultarPropostaPorProgramaResponse'=>'ConsultarPropostaPorProgramaResponse',
        'consultarPropostaPorUF'=>'ConsultarPropostaPorUF',
        'parametrosConsultarPropostaPorUFWS'=>'ParametrosConsultarPropostaPorUFWS',
        'consultarPropostaPorUFResponse'=>'ConsultarPropostaPorUFResponse',
        'enviaProponente'=>'EnviaProponente',
        'enviaProponenteResponse'=>'EnviaProponenteResponse',
        'enviaProposta'=>'EnviaProposta',
        'enviaPropostaResponse'=>'EnviaPropostaResponse',
        'exportaConvenio'=>'ExportaConvenio',
        'convenioExportacaoWS'=>'ConvenioExportacaoWS',
        'exportaConvenioResponse'=>'ExportaConvenioResponse',
        'exportaProponente'=>'ExportaProponente',
        'proponenteExportacaoWS'=>'ProponenteExportacaoWS',
        'exportaProponenteResponse'=>'ExportaProponenteResponse',
        'exportaProposta'=>'ExportaProposta',
        'exportaPropostaResponse'=>'ExportaPropostaResponse',
        'rejeitarPropostaImpedimentoTecnico'=>'RejeitarPropostaImpedimentoTecnico',
        'impedimentoTecnicoPropostaWS'=>'ImpedimentoTecnicoPropostaWS',
        'rejeitarPropostaImpedimentoTecnicoResponse'=>'RejeitarPropostaImpedimentoTecnicoResponse',
        'respostaWS'=>'RespostaWS'
    ];
}

class ObterVersaoWebService
{
}

class ObterVersaoWebServiceResponse
{
    public $return; // -- string
}

class ConsultaPropostasAlteradasOrgaoPeriodo
{
    public $arg0; // -- PropostasAlteradasExportacaoWS
}

class PropostasAlteradasExportacaoWS
{
    public $codigoOrgaoConcedente; // -- string
    public $dataInicialPeriodo; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $login; // -- LoginWS
    public $tipoPeriodo; // -- string
}

class AbstractWS
{
}

class LoginWS
{
    public $id; // -- Long
    public $senha; // -- string
    public $usuario; // -- string
}

class ConsultaPropostasAlteradasOrgaoPeriodoResponse
{
    public $return; // -- PropostasWS
}

class PropostasWS
{
    public $id; // -- Long
    public $idHash; // -- string
    public $propostas; // -- PropostaWS
}

class PropostaWS
{
    public $ano; // -- int
    public $atribuicaoRespAnalise; // -- string
    public $bensServicoWS; // -- BensServicosPropostaWS
    public $capacidadeTecnica; // -- string
    public $codigoImpedimentoTecnico; // -- string
    public $codigoOrgaoConcedente; // -- string
    public $contaBancariaWS; // -- ContaBancariaWS
    public $cronogramaDesembolsoWS; // -- CronogramaDesembolsoPropostaWS
    public $cronogramaFisicoWS; // -- CronogramaFisicoPropostaWS
    public $dataAprovacaoDePropostaWS; // -- dateTime
    public $dataAprovacaoPlanoTrabalhoWS; // -- dateTime
    public $dataEntregaProjetoBasicoWS; // -- dateTime
    public $dataLimiteComplementacaoPbWS; // -- dateTime
    public $dataLimiteEntregaProjetoBasicoWS; // -- dateTime
    public $dataPropostaWS; // -- dateTime
    public $dataUltimoEnvioContratoRepasseWS; // -- dateTime
    public $dataVersaoWS; // -- dateTime
    public $despesaAdministrativa; // -- DespesaAdministrativaWS
    public $empenhoPublicacao; // -- string
    public $executorWS; // -- ParticipantesPropostaWS
    public $fimExecucao; // -- dateTime
    public $gestao; // -- string
    public $gestaoContratoPublicacao; // -- string
    public $historico; // -- boolean
    public $id; // -- Long
    public $idHash; // -- string
    public $inicioExecucao; // -- dateTime
    public $instituicaoMandatariaWS; // -- OrgaoAdministrativoWS
    public $intervenientesProposta; // -- IntervenientePropostaWS
    public $justificativa; // -- string
    public $login; // -- LoginWS
    public $mandatarioWS; // -- ParticipantesPropostaWS
    public $membroParticipanteWS; // -- MembroParticipanteWS
    public $modalidadePropostaWS; // -- ModalidadePropostaWS
    public $motivosNaoAcatamentoContratoRepasse; // -- MotivoNaoAcatamentoContratoRepasseWS
    public $numeroControleExterno; // -- string
    public $numeroConvenio; // -- string
    public $objetoConvenio; // -- string
    public $orgaoAdministrativoWS; // -- OrgaoAdministrativoWS
    public $orgaoConcedenteWS; // -- OrgaoAdministrativoWS
    public $orgaoExecutorWS; // -- OrgaoAdministrativoWS
    public $parecerPlanoTrabalho; // -- ParecerPlanoTrabalhoWS
    public $parecerProposta; // -- ParecerPropostaConvenioWS
    public $permiteAdiarEntregaProjBasico; // -- boolean
    public $permiteLiberarPrimeiroRepasse; // -- boolean
    public $permiteProrrogarEntregaProjBasico; // -- boolean
    public $prazoEntregaProjBasico; // -- int
    public $proponenteWS; // -- ParticipantesPropostaWS
    public $propostaParticipeMembroWS; // -- ResponsaveisConvenioWS
    public $propostaProgramaWS; // -- PropostaProgramaWS
    public $repasseExercicioFuturoPropostaWS; // -- RepasseExercicioFuturoPropostaWS
    public $sequencial; // -- int
    public $situacaoEnvioContratoRepasseWS; // -- SituacaoEnvioContratoRepasseWS
    public $situacaoLegado; // -- int
    public $situacaoProjetoBasicoWS; // -- SituacaoProjetoBasicoWS
    public $situacaoPropostaWS; // -- SituacaoPropostaWS
    public $tipoNaoAcatamentoContratoRepasseWS; // -- TipoNaoAcatamentoContratoRepasseWS
    public $tipoProjetoBasicoWS; // -- TipoProjetoBasicoWS
    public $transferenciasRecursos; // -- TransferenciaRecursosWS
    public $valorContraPartida; // -- double
    public $valorContrapartidaBensServicos; // -- double
    public $valorContrapartidaFinanceira; // -- double
    public $valorGlobal; // -- double
    public $valorRepasse; // -- double
}

class BensServicosPropostaWS
{
    public $cep; // -- string
    public $descricao; // -- string
    public $despesaCompartilhada; // -- boolean
    public $endereco; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $justificativaAnalista; // -- string
    public $municipioWS; // -- MunicipioWS
    public $naturezaAquisicaoWS; // -- NaturezaAquisicaoWS
    public $naturezaDespesaSubItemWS; // -- NaturezaDespesaSubItemWS
    public $observacao; // -- string
    public $propostaProgramaWS; // -- PropostaProgramaWS
    public $quantidade; // -- double
    public $statusBensServicosPropostaWS; // -- StatusBensServicosPropostaWS
    public $tipoDespesaWS; // -- string
    public $unidadeFornecimento; // -- string
    public $valorTotal; // -- double
    public $valorUnitario; // -- double
}

class MunicipioWS
{
    public $codigo; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $nome; // -- string
    public $unidadeFederativaWS; // -- UnidadeFederativaWS
}

class UnidadeFederativaWS
{
    public $id; // -- Long
    public $idHash; // -- string
    public $sigla; // -- string
}

class NaturezaAquisicaoWS
{
    public $codigo; // -- string
    public $descricao; // -- string
    public $id; // -- Long
}

class NaturezaDespesaSubItemWS
{
    public $descricaoSubItem; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $naturezaDespesaWS; // -- NaturezaDespesaWS
    public $observacao; // -- string
    public $subItem; // -- string
}

class NaturezaDespesaWS
{
    public $codigo; // -- string
    public $descricao; // -- string
    public $id; // -- Long
    public $idHash; // -- string
}

class PropostaProgramaWS
{
    public $id; // -- Long
    public $idHash; // -- string
    public $objetoWS; // -- ObjetoWS
    public $origemRecursoPropProgramaWS; // -- OrigemRecursoPropProgramaWS
    public $programaWS; // -- ProgramaWS
    public $qualificacaoProponenteWS; // -- QualificacaoProponenteWS
    public $regrasWS; // -- RegrasWS
    public $valorContrapartida; // -- double
    public $valorContrapartidaBensServicos; // -- double
    public $valorContrapartidaFinanceira; // -- double
    public $valorGlobal; // -- double
}

class ObjetoWS
{
    public $ehObjetoPadronizado; // -- boolean
    public $id; // -- Long
    public $idHash; // -- string
    public $justificativa; // -- string
    public $nome; // -- string
}

class OrigemRecursoPropProgramaWS
{
    public $cnpjBeneficiarioEspecificoWS; // -- CnpjBeneficiarioEspecificoWS
    public $cnpjProgramaEmendaWS; // -- CnpjProgramaEmendaWS
    public $id; // -- Long
    public $idHash; // -- string
    public $qualificacaoProponenteWS; // -- string
    public $valorRepasse; // -- double
}

class CnpjBeneficiarioEspecificoWS
{
    public $cnpj; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $valorRepasseProposta; // -- double
}

class CnpjProgramaEmendaWS
{
    public $cnpj; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $numeroEmendaParlamentar; // -- string
    public $valorRepasseProposta; // -- double
}

class ProgramaWS
{
    public $acaoOrcamentaria; // -- string
    public $aceitaDespesaAdministrativa; // -- boolean
    public $aceitaPropostaDeProponenteNaoCadastrado; // -- boolean
    public $chamamentoPublico; // -- boolean
    public $cnpjBeneficiarioEspecificoWS; // -- CnpjBeneficiarioEspecificoWS
    public $cnpjProgramaEmendaWS; // -- CnpjProgramaEmendaWS
    public $codigo; // -- string
    public $concedente; // -- OrgaoAdministrativoWS
    public $criteriosDeSelecao; // -- string
    public $dataDisponibilizacao; // -- dateTime
    public $dataFimBeneficiarioEspecifico; // -- dateTime
    public $dataFimEmendaParlamentar; // -- dateTime
    public $dataFimPropostaVoluntaria; // -- dateTime
    public $dataInicioBeneficiarioEspecifico; // -- dateTime
    public $dataInicioEmendaParlamentar; // -- dateTime
    public $dataInicioPropostaVoluntaria; // -- dateTime
    public $dataPublicacaoImprensa; // -- dateTime
    public $descricao; // -- string
    public $estadosHabilitados; // -- UnidadeFederativaWS
    public $executor; // -- OrgaoAdministrativoWS
    public $id; // -- Long
    public $idHash; // -- string
    public $modalidades; // -- ModalidadeConvenioWS
    public $naturezasJuridicas; // -- NaturezaJuridicaWS
    public $nome; // -- string
    public $numeroDocumento; // -- string
    public $objetoWS; // -- ObjetoWS
    public $obrigaPlanoTrabalho; // -- boolean
    public $observacao; // -- string
    public $publicadoImprensa; // -- boolean
    public $qualificacaoBeneficiarioEmendaParlamentar; // -- boolean
    public $qualificacaoBeneficiarioEspecifico; // -- boolean
    public $qualificacaoPropostaVoluntaria; // -- boolean
    public $regrasWS; // -- RegrasWS
    public $status; // -- string
}

class OrgaoAdministrativoWS
{
    public $codigo; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $responsavelWS; // -- ResponsaveisConvenioWS
}

class ResponsaveisConvenioWS
{
    public $id; // -- Long
    public $idHash; // -- string
    public $membroWS; // -- MembroParticipanteWS
    public $participeWS; // -- ParticipantesPropostaWS
}

class MembroParticipanteWS
{
    public $ativoNoSistema; // -- boolean
    public $cargoFuncao; // -- string
    public $cep; // -- string
    public $cpf; // -- string
    public $email; // -- string
    public $endereco; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $matricula; // -- string
    public $municipioMembroWS; // -- MunicipioWS
    public $municipioWS; // -- MunicipioWS
    public $nome; // -- string
    public $orgaoExpedidor; // -- string
    public $rg; // -- string
    public $senha; // -- string
    public $telefone; // -- string
}

class ParticipantesPropostaWS
{
    public $agencia; // -- string
    public $alteracaoEstatuaria; // -- AlteracaoEstatuariaWS
    public $aprovado; // -- boolean
    public $areaAtuacaoEntidadePrivadaWS; // -- AreaAtuacaoEntidadePrivadaWS
    public $bairroDistrito; // -- string
    public $cep; // -- string
    public $certidaoAprovada; // -- boolean
    public $certidaoWS; // -- CertidaoWS
    public $cienteDirigenteNaoRemunerado; // -- boolean
    public $cnaePrimario; // -- string
    public $cnaePrimarioWS; // -- CnaeWS
    public $codigoBanco; // -- string
    public $codigoErroSiafi; // -- string
    public $consorcioPublico; // -- boolean
    public $contaCorrente; // -- string
    public $cpfUsuarioAprovou; // -- string
    public $declaracaoAprovado; // -- boolean
    public $declaracaoNaoDividaWS; // -- DeclaracaoNaoDividaWS
    public $email; // -- string
    public $endereco; // -- string
    public $entidadesVinculadas; // -- string
    public $erroSiafi; // -- string
    public $esferaAdministrativaWS; // -- EsferaAdministrativaWS
    public $estatutoAprovado; // -- boolean
    public $estatutoWS; // -- EstatutoWS
    public $executor; // -- boolean
    public $id; // -- Long
    public $idHash; // -- string
    public $identificacao; // -- string
    public $inscricaoEstadual; // -- string
    public $inscricaoMunicipal; // -- string
    public $interveniente; // -- boolean
    public $login; // -- LoginWS
    public $mandatario; // -- boolean
    public $municWS; // -- MunicipioWS
    public $naturezaJuridica; // -- string
    public $nome; // -- string
    public $nomeFantasia; // -- string
    public $proponente; // -- boolean
    public $quadroDirigenteAprovado; // -- boolean
    public $representanteWS; // -- MembroParticipanteWS
    public $respExercicioWS; // -- MembroParticipanteWS
    public $responsavelWS; // -- MembroParticipanteWS
    public $respostaEnvioSiafi; // -- string
    public $statusEnvioSiafi; // -- string
    public $telefone; // -- string
    public $telexFaxCaixaPostal; // -- string
    public $tipoIdentificacaoWS; // -- TipoIdentificadorParticipeWS
    public $transcricaoEstatutoONG; // -- string
}

class AlteracaoEstatuariaWS
{
    public $data; // -- dateTime
    public $id; // -- Long
    public $idHash; // -- string
    public $texto; // -- string
}

class AreaAtuacaoEntidadePrivadaWS
{
    public $codigo; // -- string
    public $descricao; // -- string
    public $id; // -- Long
    public $idHash; // -- string
}

class CertidaoWS
{
    public $fgtsDataEmissao; // -- dateTime
    public $fgtsDataValidade; // -- dateTime
    public $fgtsHora; // -- string
    public $fgtsIsento; // -- boolean
    public $fgtsNumero; // -- string
    public $fgtsTipo; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $inssDataEmissao; // -- dateTime
    public $inssDataValidade; // -- dateTime
    public $inssHora; // -- string
    public $inssIsento; // -- boolean
    public $inssNumero; // -- string
    public $inssTipo; // -- string
    public $outrasCertidoesWS; // -- RespostaCertidaoWS
    public $receitaEstadualDataEmissao; // -- dateTime
    public $receitaEstadualDataValidade; // -- dateTime
    public $receitaEstadualHora; // -- string
    public $receitaEstadualIsento; // -- boolean
    public $receitaEstadualNumero; // -- string
    public $receitaEstadualTipo; // -- string
    public $receitaMunicipalDataEmissao; // -- dateTime
    public $receitaMunicipalDataValidade; // -- dateTime
    public $receitaMunicipalHora; // -- string
    public $receitaMunicipalIsento; // -- boolean
    public $receitaMunicipalNumero; // -- string
    public $receitaMunicipalTipo; // -- string
    public $srfPgfnDataEmissao; // -- dateTime
    public $srfPgfnDataValidade; // -- dateTime
    public $srfPgfnHora; // -- string
    public $srfPgfnIsento; // -- boolean
    public $srfPgfnNumero; // -- string
    public $srfPgfnTipo; // -- string
}

class RespostaCertidaoWS
{
    public $dataValidade; // -- dateTime
    public $id; // -- Long
    public $idHash; // -- string
    public $nomeCertidao; // -- string
    public $possuiCertidao; // -- boolean
}

class CnaeWS
{
    public $codigo; // -- string
    public $descricao; // -- string
    public $id; // -- Long
    public $idHash; // -- string
}

class DeclaracaoNaoDividaWS
{
    public $dataAssinatura; // -- dateTime
    public $dirigenteSignatarioWS; // -- DirigenteWS
    public $id; // -- Long
    public $idHash; // -- string
}

class DirigenteWS
{
    public $cargo; // -- string
    public $cpf; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $nome; // -- string
    public $orgaoExpedidor; // -- string
    public $profissao; // -- string
    public $rg; // -- string
}

class EsferaAdministrativaWS
{
    public $codigo; // -- string
    public $id; // -- Long
}

class EstatutoWS
{
    public $cartorio; // -- string
    public $dataRegistro; // -- dateTime
    public $id; // -- Long
    public $idHash; // -- string
    public $livroFolha; // -- string
    public $municipioWS; // -- MunicipioWS
    public $numeroDoRegistroMatricula; // -- string
    public $transcricaoEstatuto; // -- string
}

class TipoIdentificadorParticipeWS
{
    public $codigo; // -- string
    public $id; // -- Long
}

class ModalidadeConvenioWS
{
    public $id; // -- Long
    public $idHash; // -- string
    public $valor; // -- string
}

class NaturezaJuridicaWS
{
    public $id; // -- Long
    public $idHash; // -- string
    public $valor; // -- string
}

class RegrasWS
{
    public $aceitaContrapartidaBens; // -- boolean
    public $descricao; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $percentualMaximoContrapartidaBens; // -- double
    public $percentualMinimoContrapartida; // -- double
}

class QualificacaoProponenteWS
{
    public $id; // -- Long
    public $value; // -- string
}

class StatusBensServicosPropostaWS
{
    public $codigo; // -- string
    public $id; // -- Long
}

class ContaBancariaWS
{
    public $agencia; // -- string
    public $bancoWS; // -- BancoWS
    public $conta; // -- string
    public $dataRetornoUltimoPedidoAberturaWS; // -- dateTime
    public $dataUltimaModificacaoWS; // -- dateTime
    public $dataUltimoEnvioPedidoAberturaWS; // -- dateTime
    public $descricao; // -- string
    public $digitoVerificadorAgencia; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $mensagemErro; // -- string
    public $msgErroProcessamento; // -- string
    public $situacao; // -- string
}

class BancoWS
{
    public $codigo; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $nome; // -- string
}

class CronogramaDesembolsoPropostaWS
{
    public $id; // -- Long
    public $idHash; // -- string
    public $parcelaCronogramaDesembolsoWS; // -- ParcelaCronogramaDesembolsoWS
}

class ParcelaCronogramaDesembolsoWS
{
    public $ano; // -- Long
    public $id; // -- Long
    public $idHash; // -- string
    public $mes; // -- Long
    public $numero; // -- Long
    public $parcelaMetaWS; // -- ParcelaMetaWS
    public $tipoParticipanteWS; // -- TipoParticipanteWS
    public $valor; // -- double
}

class ParcelaMetaWS
{
    public $id; // -- Long
    public $idHash; // -- string
    public $idMetaCronogramaFisico; // -- Long
    public $parcelaEtapaWS; // -- ParcelaEtapaWS
    public $valor; // -- double
}

class ParcelaEtapaWS
{
    public $id; // -- Long
    public $idEtapaCronogramaFisico; // -- Long
    public $idHash; // -- string
    public $valor; // -- double
}

class TipoParticipanteWS
{
    public $codigo; // -- string
    public $descricao; // -- string
    public $id; // -- Long
}

class CronogramaFisicoPropostaWS
{
    public $id; // -- Long
    public $idHash; // -- string
    public $metaCronogramaFisicoWS; // -- MetaCronogramaFisicoWS
}

class MetaCronogramaFisicoWS
{
    public $cep; // -- string
    public $dataFim; // -- dateTime
    public $dataInicio; // -- dateTime
    public $endereco; // -- string
    public $especificacao; // -- string
    public $etapaCronogramaFisicoWS; // -- EtapaCronogramaFisicoWS
    public $id; // -- Long
    public $idHash; // -- string
    public $municipioWS; // -- MunicipioWS
    public $propostaProgramaWS; // -- PropostaProgramaWS
    public $quantidade; // -- double
    public $tipoMeta; // -- string
    public $unidadeFornecimento; // -- string
    public $valor; // -- double
}

class EtapaCronogramaFisicoWS
{
    public $cep; // -- string
    public $dataFim; // -- dateTime
    public $dataInicio; // -- dateTime
    public $endereco; // -- string
    public $especificacao; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $municipioWS; // -- MunicipioWS
    public $quantidade; // -- double
    public $unidadeFornecimento; // -- string
    public $valor; // -- double
}

class DespesaAdministrativaWS
{
    public $cep; // -- string
    public $compartilhada; // -- boolean
    public $descricao; // -- string
    public $endereco; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $municipio; // -- MunicipioWS
    public $naturezaAquisicao; // -- string
    public $naturezaDespesaSubItem; // -- NaturezaDespesaSubItemWS
    public $numeroConvenio; // -- string
    public $observacao; // -- string
    public $programa; // -- ProgramaWS
    public $quantidade; // -- double
    public $tipoDespesa; // -- string
    public $unidadeFornecimento; // -- string
    public $valorUnitario; // -- double
}

class IntervenientePropostaWS
{
    public $orgaoAdministrativo; // -- OrgaoAdministrativoWS
    public $participanteProposta; // -- ParticipantesPropostaWS
}

class ModalidadePropostaWS
{
    public $id; // -- Long
    public $value; // -- string
}

class MotivoNaoAcatamentoContratoRepasseWS
{
    public $id; // -- Long
    public $idHash; // -- string
    public $value; // -- string
}

class ParecerPlanoTrabalhoWS
{
    public $atribuicaoResponsavel; // -- string
    public $data; // -- dateTime
    public $favoravelWS; // -- string
    public $funcaoResponsavel; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $parecer; // -- string
    public $responsavelWS; // -- MembroParticipanteWS
    public $tipoParecerWS; // -- string
    public $tipoParticipanteWS; // -- string
}

class ParecerPropostaConvenioWS
{
    public $atribuicaoResponsavel; // -- string
    public $dataWS; // -- dateTime
    public $funcaoResponsavel; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $parecer; // -- string
    public $responsavelWS; // -- MembroParticipanteWS
    public $tipoParticipanteWS; // -- string
}

class RepasseExercicioFuturoPropostaWS
{
    public $ano; // -- int
    public $historico; // -- boolean
    public $id; // -- Long
    public $idHash; // -- string
    public $valor; // -- double
}

class SituacaoEnvioContratoRepasseWS
{
    public $id; // -- Long
    public $value; // -- string
}

class SituacaoProjetoBasicoWS
{
    public $id; // -- Long
    public $value; // -- string
}

class SituacaoPropostaWS
{
    public $id; // -- Long
    public $value; // -- string
}

class TipoNaoAcatamentoContratoRepasseWS
{
    public $id; // -- Long
    public $value; // -- string
}

class TipoProjetoBasicoWS
{
    public $id; // -- Long
    public $value; // -- string
}

class TransferenciaRecursosWS
{
    public $ano; // -- Long
    public $codigoCancelamento; // -- string
    public $codigoErroSIAFI; // -- string
    public $codigoGestaoEmitente; // -- string
    public $codigoUgEmitente; // -- string
    public $data; // -- dateTime
    public $dataCancelamento; // -- dateTime
    public $erroSIAFI; // -- string
    public $gestaoUgFavorecida; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $numeroCancelamento; // -- string
    public $numeroInterno; // -- Long
    public $numeroSIAFI; // -- string
    public $OBConfluxoExportacao; // -- ObConfluxoExportacaoWS
    public $observacao; // -- string
    public $observacaoCancelamento; // -- string
    public $ordemBancariaExportacao; // -- OrdemBancariaExportacaoWS
    public $programacaoFinanceiraExportacao; // -- ProgramacaoFinanceiraExportacaoWS
    public $valor; // -- double
}

class ObConfluxoExportacaoWS
{
    public $anoDocDh; // -- string
    public $dadosOrcamentariosWS; // -- DadosOrcamentariosWS
    public $gestaoEmitenteDh; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $numeroDh; // -- string
    public $textoJustificativaInadimplencia; // -- string
    public $tipoDh; // -- string
    public $ugEmitenteDh; // -- string
}

class DadosOrcamentariosWS
{
    public $categoriaGasto; // -- boolean
    public $id; // -- Long
    public $idHash; // -- string
    public $numeroEmpenho; // -- string
    public $valorGrupo; // -- double
    public $vinculacao; // -- string
}

class OrdemBancariaExportacaoWS
{
    public $codigoAgenciaDestino; // -- string
    public $codigoAgenciaOrigem; // -- string
    public $codigoBancoDestino; // -- string
    public $codigoBancoOrigem; // -- string
    public $codigoFavorecido; // -- string
    public $controleStnOriginal; // -- string
    public $dataEnvioXml; // -- dateTime
    public $eventosObWS; // -- EventoOrdemBancariaWS
    public $gestaoDocumentoOrigem; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $identificadorTransferencia; // -- string
    public $numeroContaCorrenteDestino; // -- string
    public $numeroContaCorrenteOrigem; // -- string
    public $numeroDocumentoOrigem; // -- string
    public $numeroInternoConcedente; // -- string
    public $numeroListaOb; // -- string
    public $numeroProcesso; // -- string
    public $opcaoInversaoSaldo; // -- boolean
    public $taxaCambio; // -- double
    public $ugDocumentoOrigem; // -- string
}

class EventoOrdemBancariaWS
{
    public $classificacao1; // -- string
    public $classificacao2; // -- string
    public $classificacaoOrc1; // -- string
    public $classificacaoOrc2; // -- string
    public $codEvento; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $inscricao1; // -- string
    public $inscricao2; // -- string
    public $valorLancamento; // -- double
}

class ProgramacaoFinanceiraExportacaoWS
{
    public $codigoFavorecidoOB; // -- string
    public $codigoGestaoFavorecidaOB; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $ugFavorecido; // -- Long
}

class ConsultaPropostasOrgao
{
    public $arg0; // -- PropostaExportacaoWS
}

class PropostaExportacaoWS
{
    public $ano; // -- int
    public $codigoOrgaoConcedente; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $login; // -- LoginWS
    public $sequencial; // -- int
}

class ConsultaPropostasOrgaoResponse
{
    public $return; // -- RetornoConsultaPropostasOrgaoWS
}

class RetornoConsultaPropostasOrgaoWS
{
    public $id; // -- Long
    public $idHash; // -- string
    public $propostaConsultaWS; // -- PropostaConsultaWS
}

class PropostaConsultaWS
{
    public $codigoPrograma; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $identificacaoProponente; // -- string
    public $numeroProposta; // -- string
    public $tipoIdentificacaoProponente; // -- string
}

class ConsultarConveniosOrgaoPorAno
{
    public $arg0; // -- ParametrosConsultarConveniosOrgaoPorAnoWS
}

class ParametrosConsultarConveniosOrgaoPorAnoWS
{
    public $ano; // -- int
    public $codigoOrgaoConcedente; // -- string
    public $login; // -- LoginWS
}

class ConsultarConveniosOrgaoPorAnoResponse
{
    public $return; // -- ConvenioWS
}

class ConvenioWS
{
    public $ano; // -- int
    public $dataAssinatura; // -- dateTime
    public $dataPrestacaoContas; // -- dateTime
    public $dataPublicacao; // -- dateTime
    public $empenhado; // -- boolean
    public $fimExecucao; // -- dateTime
    public $fundamentoLegal; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $inadimplente; // -- boolean
    public $inicioExecucao; // -- dateTime
    public $justificativa; // -- string
    public $justificativaAlteracaoAdimplencia; // -- string
    public $login; // -- LoginWS
    public $notaEmpenhoWS; // -- NotaEmpenhoWS
    public $notaLancamentoWS; // -- NotaLancamentoWS
    public $numeroControleExterno; // -- string
    public $numeroInterno; // -- string
    public $numeroProcesso; // -- string
    public $permiteAjustesCronFisico; // -- boolean
    public $prazoPrestacaoContas; // -- int
    public $prazoProrrogacaoPrestacaoContas; // -- int
    public $propostaConvenioWS; // -- PropostaConvenioWS
    public $propostaWS; // -- PropostaWS
    public $prorrogasOficio; // -- ProrrogaOficioWS
    public $publicado; // -- boolean
    public $sequencial; // -- int
    public $situacaoPublicacaoWS; // -- string
    public $statusConvenioWS; // -- string
    public $termosAditivosWS; // -- TermoAditivoWS
    public $tipoBloqueioWS; // -- string
    public $tipoConvenioWS; // -- string
}

class NotaEmpenhoWS
{
    public $amparoLegal; // -- string
    public $ano; // -- string
    public $codigoErroSiafi; // -- string
    public $contaCorrenteFinanceiro; // -- string
    public $contaCorrenteWS; // -- ContaCorrenteWS
    public $contaPassivo; // -- string
    public $cronogramaEmpenhoWS; // -- CronogramaEmpenhoWS
    public $dataEmissao; // -- dateTime
    public $dataEnvioXml; // -- dateTime
    public $dataPagamento; // -- dateTime
    public $erroSiafi; // -- string
    public $esferaOrcamentaria; // -- string
    public $especieEmpenho; // -- TipoNotaEmpenhoWS
    public $favorecido; // -- string
    public $fonteRecurso; // -- string
    public $gestaoEmitente; // -- string
    public $gestaoEmpenhoOriginal; // -- string
    public $gestaoFavorecida; // -- string
    public $gestaoReferencia; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $inciso; // -- string
    public $indicadorContraEntregaWS; // -- string
    public $indicadorResultado; // -- string
    public $indicadorTipoCredito; // -- string
    public $itensEmpenhoWS; // -- ItemEmpenhoWS
    public $modalidadeLicitacao; // -- Long
    public $municipioBeneficiado; // -- string
    public $numero; // -- string
    public $numeroConvenio; // -- string
    public $numeroEmpenhoOriginal; // -- string
    public $numeroInterno; // -- Long
    public $numeroInternoConcedente; // -- string
    public $numeroLista; // -- string
    public $objeto; // -- string
    public $observacao; // -- string
    public $origemMaterial; // -- string
    public $passivoAnterior; // -- string
    public $planoInterno; // -- string
    public $programaDeTrabalho; // -- string
    public $ptres; // -- string
    public $situacaoWS; // -- string
    public $taxaCambio; // -- double
    public $tipoEmpenhoWS; // -- TipoEmpenhoWS
    public $ugEmitente; // -- string
    public $ugEmpenhoOriginal; // -- string
    public $ugReferencia; // -- string
    public $ugr; // -- string
    public $unidadeFederativaWS; // -- UnidadeFederativaWS
    public $unidadeOrcamentaria; // -- string
    public $valorEmpenho; // -- double
}

class ContaCorrenteWS
{
    public $contaCorrentePermanente; // -- string
    public $valorContaCorrentePermanente; // -- double
}

class CronogramaEmpenhoWS
{
    public $dataRecebimento; // -- dateTime
    public $dataVencimento; // -- dateTime
    public $id; // -- Long
    public $idHash; // -- string
    public $valor; // -- double
}

class TipoNotaEmpenhoWS
{
    public $codigo; // -- string
    public $id; // -- Long
}

class ItemEmpenhoWS
{
    public $descricao; // -- string
    public $especie; // -- int
    public $id; // -- Long
    public $idHash; // -- string
    public $naturezaDespesaSubItem; // -- string
    public $quantidade; // -- double
    public $referencia; // -- string
    public $valorUnitario; // -- double
}

class TipoEmpenhoWS
{
    public $codigo; // -- string
    public $id; // -- Long
    public $idHash; // -- string
}

class NotaLancamentoWS
{
    public $ano; // -- string
    public $codErroSiafi; // -- string
    public $dataEmissao; // -- dateTime
    public $dataEnvioXml; // -- dateTime
    public $dataValorizacao; // -- dateTime
    public $dataVencimentoTituloCredito; // -- dateTime
    public $erroSiafi; // -- string
    public $eventoNotaLancamentoWS; // -- EventoNotaLancamentoWS
    public $favorecido; // -- string
    public $gestaoEmitente; // -- string
    public $gestaoFavorecida; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $numConvenio; // -- string
    public $numeroControleExterno; // -- string
    public $numeroDocumento; // -- string
    public $numeroInterno; // -- Long
    public $numeroInternoConcedente; // -- string
    public $observacao; // -- string
    public $selecionadoDh; // -- string
    public $situacao; // -- string
    public $taxaCambio; // -- double
    public $tituloCredito; // -- string
    public $uasgEmitente; // -- string
    public $ugEmitente; // -- string
}

class EventoNotaLancamentoWS
{
    public $classificacao1; // -- string
    public $classificacao2; // -- string
    public $classificacaoOrc1; // -- string
    public $classificacaoOrc2; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $inscricao1; // -- string
    public $inscricao2; // -- string
    public $numero; // -- string
    public $valorLancamento; // -- double
}

class PropostaConvenioWS
{
    public $ano; // -- int
    public $id; // -- Long
    public $idHash; // -- string
    public $sequencial; // -- int
}

class ProrrogaOficioWS
{
    public $assinado; // -- boolean
    public $cpfResponsavelAssinatura; // -- string
    public $dataAssinatura; // -- dateTime
    public $dataCadastro; // -- dateTime
    public $dataDisponibilizacao; // -- dateTime
    public $dataFimVigencia; // -- dateTime
    public $dataPublicacao; // -- dateTime
    public $diasProrrogacao; // -- int
    public $finalizado; // -- boolean
    public $id; // -- Long
    public $idHash; // -- string
    public $nomeResponsavelAssinatura; // -- string
    public $numero; // -- string
    public $publicado; // -- boolean
    public $publicar; // -- boolean
    public $responsavelRegistroWS; // -- MembroParticipanteWS
    public $statusWS; // -- string
}

class TermoAditivoWS
{
    public $ano; // -- int
    public $assinado; // -- boolean
    public $codigoEnvioSiafi; // -- Long
    public $codigoEnvioSiafiContrapartida; // -- Long
    public $codigoErroSiafi; // -- string
    public $data; // -- dateTime
    public $dataAssinatura; // -- dateTime
    public $dataEnvioXml; // -- dateTime
    public $dataFimVigencia; // -- dateTime
    public $dataInicioVigencia; // -- dateTime
    public $dataPublicacao; // -- dateTime
    public $descricaoOutros; // -- string
    public $erroSiafi; // -- string
    public $finalizado; // -- boolean
    public $fundamentoLegal; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $justificativa; // -- string
    public $numeroNS; // -- string
    public $numeroNSContrapartida; // -- string
    public $objetoAlteracao; // -- string
    public $permiteAlteracoes; // -- boolean
    public $permitirEdicaoConvenente; // -- boolean
    public $publicado; // -- boolean
    public $publicar; // -- boolean
    public $repassesExercicioFuturosTAWS; // -- RepasseExercicioFuturoTermoAditivoWS
    public $respConcedenteWS; // -- MembroParticipanteWS
    public $respRegistroWS; // -- MembroParticipanteWS
    public $sequencial; // -- int
    public $situacaoWS; // -- string
    public $statusWS; // -- string
    public $textoAmpliacaoObjeto; // -- string
    public $tiposTAWS; // -- TipoTaWS
    public $valorContrapartida; // -- double
    public $valorContrapartidaBensServicos; // -- double
    public $valorContrapartidaFinanceira; // -- double
    public $valorGlobal; // -- double
    public $valorRepasse; // -- double
}

class RepasseExercicioFuturoTermoAditivoWS
{
    public $ano; // -- int
    public $id; // -- Long
    public $idHash; // -- string
    public $valor; // -- double
}

class TipoTaWS
{
    public $codigoTipo; // -- string
    public $id; // -- Long
    public $idHash; // -- string
}

class ConsultarPrograma
{
    public $arg0; // -- ParametrosConsultarProgramaWS
}

class ParametrosConsultarProgramaWS
{
    public $codigoPrograma; // -- string
    public $id; // -- Long
    public $login; // -- LoginWS
}

class ConsultarProgramaResponse
{
    public $return; // -- ProgramaWS
}

class ConsultarPropostaPorAcaoOrcamentaria
{
    public $arg0; // -- ParametrosConsultarPropostaPorAcaoOrcamentariaWS
}

class ParametrosConsultarPropostaPorAcaoOrcamentariaWS
{
    public $codigoOrgao; // -- string
    public $id; // -- Long
    public $login; // -- LoginWS
    public $numeroAcaoOrcamentaria; // -- string
}

class ConsultarPropostaPorAcaoOrcamentariaResponse
{
    public $return; // -- PropostasWS
}

class ConsultarPropostaPorCNPJ
{
    public $arg0; // -- ParametrosConsultarPropostaPorCNPJWS
}

class ParametrosConsultarPropostaPorCNPJWS
{
    public $cnpj; // -- string
    public $codigoOrgao; // -- string
    public $codigoPrograma; // -- string
    public $id; // -- Long
    public $login; // -- LoginWS
}

class ConsultarPropostaPorCNPJResponse
{
    public $return; // -- PropostasWS
}

class ConsultarPropostaPorEmendaParlamentar
{
    public $arg0; // -- ParametrosConsultarPropostaPorEmendaParlamentarWS
}

class ParametrosConsultarPropostaPorEmendaParlamentarWS
{
    public $codigoOrgao; // -- string
    public $id; // -- Long
    public $login; // -- LoginWS
    public $numeroEmendaParlamentar; // -- string
}

class ConsultarPropostaPorEmendaParlamentarResponse
{
    public $return; // -- PropostasWS
}

class ConsultarPropostaPorMunicipio
{
    public $arg0; // -- ParametrosConsultarPropostaPorMunicipioWS
}

class ParametrosConsultarPropostaPorMunicipioWS
{
    public $codigoMunicipio; // -- string
    public $codigoOrgao; // -- string
    public $id; // -- Long
    public $login; // -- LoginWS
}

class ConsultarPropostaPorMunicipioResponse
{
    public $return; // -- PropostasWS
}

class ConsultarPropostaPorPrograma
{
    public $arg0; // -- ParametrosConsultarPropostaPorProgramaWS
}

class ParametrosConsultarPropostaPorProgramaWS
{
    public $codigoOrgao; // -- string
    public $id; // -- Long
    public $login; // -- LoginWS
    public $numeroPrograma; // -- string
}

class ConsultarPropostaPorProgramaResponse
{
    public $return; // -- PropostasWS
}

class ConsultarPropostaPorUF
{
    public $arg0; // -- ParametrosConsultarPropostaPorUFWS
}

class ParametrosConsultarPropostaPorUFWS
{
    public $codigoOrgao; // -- string
    public $codigoUF; // -- string
    public $id; // -- Long
    public $login; // -- LoginWS
}

class ConsultarPropostaPorUFResponse
{
    public $return; // -- PropostasWS
}

class EnviaProponente
{
    public $arg0; // -- ParticipantesPropostaWS
}

class EnviaProponenteResponse
{
    public $return; // -- string
}

class EnviaProposta
{
    public $arg0; // -- PropostaWS
}

class EnviaPropostaResponse
{
    public $return; // -- string
}

class ExportaConvenio
{
    public $arg0; // -- ConvenioExportacaoWS
}

class ConvenioExportacaoWS
{
    public $ano; // -- int
    public $codigoOrgaoConcedente; // -- string
    public $id; // -- Long
    public $idHash; // -- string
    public $login; // -- LoginWS
    public $sequencial; // -- int
}

class ExportaConvenioResponse
{
    public $return; // -- ConvenioWS
}

class ExportaProponente
{
    public $arg0; // -- ProponenteExportacaoWS
}

class ProponenteExportacaoWS
{
    public $id; // -- Long
    public $identificacao; // -- string
    public $login; // -- LoginWS
    public $tipoIdentificacao; // -- string
}

class ExportaProponenteResponse
{
    public $return; // -- ParticipantesPropostaWS
}

class ExportaProposta
{
    public $arg0; // -- PropostaExportacaoWS
}

class ExportaPropostaResponse
{
    public $return; // -- PropostaWS
}

class RejeitarPropostaImpedimentoTecnico
{
    public $arg0; // -- ImpedimentoTecnicoPropostaWS
}

class ImpedimentoTecnicoPropostaWS
{
    public $anoProposta; // -- int
    public $codigoImpedimentoTecnico; // -- string
    public $codigoOrgaoConcedenteProposta; // -- string
    public $justificativa; // -- string
    public $login; // -- LoginWS
    public $sequencialProposta; // -- int
}

class RejeitarPropostaImpedimentoTecnicoResponse
{
    public $return; // -- RespostaWS
}

class RespostaWS
{
    public $identificador; // -- string
    public $mensagem; // -- string
    public $sucesso; // -- boolean
}