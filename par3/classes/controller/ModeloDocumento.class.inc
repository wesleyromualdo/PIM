<?php
class Par3_Controller_Modelo_Documento extends Controle
{
	private $model;

	public function __construct()
	{
		$this->model = new Par3_Model_Modelo_Documento();
	}
	
	public function salvar_modelo( $arrPost = array() ){
		$arrPost['usucpf'] = $_SESSION['usucpf'];
		
		if( strpos($arrPost["mdoconteudo"], '<p style=\"page-break-before: always;\"><!-- pagebreak --></p>') ) {
			$arrPost["mdoconteudo"] = str_replace('<p style=\"page-break-before: always;\"><!-- pagebreak --></p>', '<p style="page-break-before:always"><!-- pagebreak --></p>', $arrPost["mdoconteudo"] );
		} else {
			$arrPost["mdoconteudo"] = str_replace("<!-- pagebreak -->", '<p style="page-break-before:always"><!-- pagebreak --></p>', $arrPost["mdoconteudo"] );
		}
		
		$gtdValidacao = 1;
		if( !empty($arrPost["iutid"]) ){
			$gtdValidacao = 2;
		}
		$arrPost["mdoqtdvalidacao"] = $gtdValidacao;
		
		if( $arrPost['tpdcod_a'] ) $arrPost["mdotipovinculado"] = $arrPost['tpdcod_a'];
		
		$arrPost["mdoconteudo"] = substituir_char_especiais_word($arrPost["mdoconteudo"]);
		$arrPost["mdoconteudo"] = str_replace('\"', '"', $arrPost["mdoconteudo"]);
		$arrPost["mdoconteudo"] = str_replace("\'", "'", $arrPost["mdoconteudo"]);
		
		$arrPost['arChavePrimaria'] = $this->model->arChavePrimaria[0];
		$arrPost['stNomeTabela'] = $this->model->stNomeTabela;
		$arrPost['arAtributosTabela'] = $this->model->arAtributosTabela;
		
		$chave = trata_salvar($arrPost);
		
		$url = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=modelodocumento';
		
		if( (int)$chave > 0 ){
			simec_redirecionar($url, 'success', 'Modelo de Documento Gerado com Sucesso');
		} else {
			simec_redirecionar($url, 'error', 'Erro ao Gerar o Modelo de Documento');
		}
	}
	public function listar( $arrPost = array() ){
		
		$sql = $this->model->listar( $arrPost );
	
		$listagemSimec = new Simec_Listagem();
		$arrayCabecalho = array('Código', 'Modelo', 'Tipo Documento', 'De Ofício', 'Tipo', 'Esfera', 'Validação Gestor', 'Validação FNDE', 'Data Inclusão', 'Usuário');
		$esconderColunas = array();
		$listagemSimec->setCabecalho($arrayCabecalho);
		
		$listagemSimec->esconderColunas($esconderColunas);
		$listagemSimec->turnOnPesquisator();
		$listagemSimec->setQuery($sql);
		$listagemSimec->addAcao('edit', 'editarTermoGerado');
		$listagemSimec->addAcao('delete', 'deletarTermoGerado');
		$listagemSimec->addAcao('view', 'visualizarTermoGerado');
		$listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
		$listagemSimec->setTamanhoPagina(100);
		$listagemSimec->setCampos($arrayCabecalho);
		
		$listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
	}
	
	public function carregaModeloDocumento( $mdoid ){
		$sql = "SELECT mdoid, tpdcod, mdonome, mdoconteudo, mdostatus, usucpf, mdodatainclusao,
				       mdovalidacaogestor, mdovalidacaofnde, mdoqtdvalidacao, mdotipovinculado,
				       mdovisivel, iutid, mdoex_oficio, mdoesfera, mdotipo
				  FROM par3.modelodocumento WHERE mdoid = $mdoid";
		$arrDados = $this->model->pegaLinha($sql);
		return $arrDados;
	}
	
	public function excluirModeloDocumento( $mdoid ){
		$sql = "UPDATE par3.modelodocumento SET mdostatus = 'I' WHERE mdoid = $mdoid";
		$this->model->executar($sql);
		
		$url = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=modelodocumento';
		
		if( $this->model->commit() ){
			simec_redirecionar($url, 'success', 'Modelo de Documento Excluido com Sucesso');
		} else {
			simec_redirecionar($url, 'error', 'Erro ao Excluir o Modelo de Documento');
		}
		return true;
	}
}