<?php
/**
 * Classe de mapeamento da entidade par3.obra_tipo_documento.
 *
 * @version $Id$
 * @since 2017.07.05
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_ObraTipoDocumento: Tabela utilizada na funcionalidade: H11029 Manter Tabela de Apoio - Documentos Anexos
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Philipe Allan Almeida <philipe.almeida@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_ObraTipoDocumento($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_ObraTipoDocumento($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $otdid Chave primária da tabela - default: nextval('par3.obra_tipo_documento_otdid_seq'::regclass)
 * @property int $extid FK da Extensão de Arquivo (public.extensao_arquivo)
 * @property string $otddsc Descrição do Documentos Anexo
 * @property string $otdajuda Texto de Ajuda
 * @property string $otdsituacao Situação com as opções A = Ativo e I = Inativo
 * @property string $otdstatus Status do registro A = ativo e I = inativo - default: 'A'::bpchar
 * @property int $otdtamanho_minimo Quantidade Mínima para o tamanho do arquivo em Kb.
 * @property int $otdtamanho_maximo Quantidade Máxima do tamaho do arquivo em Kb.
 * @property int $otdqtd_minima Quantidade mínima de arquivos
 * @property int $otdqtd_maximo Quantidade máxima de arquivos.
 * @property string $otdanalise_engenharia descrição da pergunta da análise de engenharia.
 * @property char $otdanalise_textorico valida se o campo texto da análise será texto rico ou não.
 */
class Par3_Model_ObraTipoDocumento extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_tipo_documento';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'otdid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'extid' => array('tabela' => 'extensao_arquivo', 'pk' => 'extid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'otdid' => null,
        'arqid' => null,
        'otddsc' => null,
        'otdajuda' => null,
        'otdsituacao' => null,
        'otdstatus' => null,
        'otdtamanho_minimo' => null,
        'otdtamanho_maximo' => null,
        'otdqtd_minima' => null,
        'otdqtd_maximo' => null,
        'otdtodas_extensoes' => null,
        'otdanalise_engenharia' => null,
        'otdanalise_textorico' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'otdid' => array('Digits'),
            'extid' => array('allowEmpty' => true,'Digits'),
            'otddsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'otdajuda' => array('allowEmpty' => true),
            'otdsituacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'otdstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'otdtamanho_minimo' => array('allowEmpty' => true,'Digits'),
            'otdtamanho_maximo' => array('allowEmpty' => true,'Digits'),
            'otdqtd_minima' => array('allowEmpty' => true,'Digits'),
            'otdqtd_maximo' => array('allowEmpty' => true,'Digits'),
            'otdanalise_engenharia' => array('allowEmpty' => true),
            'otdanalise_textorico' => array('allowEmpty' => true),
        );
    }

    public function retornaSQLListaSimples($dados = array())
    {
        $pesquisaCodigo = '';
        $pesquisaDescricao = '';

        if (isset($dados['acao']) && $dados['acao'] == 'pesquisar'){

            if (strlen($dados['codigo']) > 0){
                $pesquisaCodigo = ' AND otdid = '.$dados['codigo'];
            }

            if (strlen($dados['descricao']) > 0){
                $pesquisaDescricao = " AND otddsc ILIKE '%".trim($dados['descricao'])."%' ";
            }
        }
		
//         $camposSelect = "
//         			    otdid as id, 
//                         otdid as codigo, 
//                         otddsc as otddsc, 
//                         CASE
//                           WHEN (otdsituacao = 'A') THEN 'Ativo'
//                           ELSE 'Inativo'
//                          END AS otdsituacao, 
//                         CASE
//                           WHEN (otdtodas_extensoes = TRUE) THEN 'Todos'
//                           ELSE 
//                             array_to_string(
//                                 ARRAY(
//                                 SELECT extdesc 
//                                 FROM par3.obra_tipo_documento_x_extensao as tipxenx 
//                                 INNER JOIN extensao_arquivo 
//                                     ON (tipxenx.extid = extensao_arquivo.extid)  
//                                 WHERE otdid = doc.otdid
//                                 ), ', '
//                             ) 
//                          END AS extensoes,
//                         otdtamanho_maximo, 
//                         'De '||otdqtd_minima||' a '||otdqtd_maximo as quatidade
//         ";
//         $orderBy = "ORDER BY 1";
        
//         if($dados['requisicao'] == 'imprimir' || $dados['requisicao'] == 'xls'){
//         	$contador = "ROW_NUMBER() OVER ({$orderBy}) AS qtd,";
//         	$inicio = " 			
//         			SELECT ROW_NUMBER() OVER ({$orderBy}) AS qtd,
//         			*FROM (
//         	";
        	
//         	$fim = ") as foo";
//         } else {
//         	$contador = '';
//         	$inicio = '';
//         	$fim = '';
//         }
        
//         $select = "SELECT $camposSelect";
        
//         $sql = "
//         	{$inicio}
//         	{$select}
//             FROM par3.obra_tipo_documento as doc
//             WHERE otdstatus = 'A' " . $pesquisaCodigo . $pesquisaDescricao . "
//             {$orderby}
//             {$fim}
//         ";
            
            $sql = "SELECT 
						otdid as id, 
						ROW_NUMBER() OVER (ORDER BY 1) AS qtd,
						otdid as codigo, 
						otddsc as otddsc, 
						CASE
							WHEN (otdsituacao = 'A') THEN 'Ativo'
							ELSE 'Inativo'
						END AS otdsituacao, 
						CASE
							WHEN (otdtodas_extensoes = TRUE) THEN 'Todos'
							ELSE 
								array_to_string(
									ARRAY(
										SELECT extdesc 
										FROM par3.obra_tipo_documento_x_extensao as tipxenx 
										INNER JOIN extensao_arquivo ON (tipxenx.extid = extensao_arquivo.extid)  
										WHERE otdid = doc.otdid
									), ', '
								) 
						END AS extensoes,
						otdtamanho_maximo, 
						'De '||otdqtd_minima||' a '||otdqtd_maximo as quatidade,
						otdanalise_engenharia
					FROM par3.obra_tipo_documento as doc
					WHERE otdstatus = 'A' ".$pesquisaCodigo.$pesquisaDescricao;
            
		//print_r($sql); exit();
        return $sql;
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        if(!isset($_POST['otdanalise_textorico'])){
            $_POST['otdanalise_textorico'] = 'N';
        }
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

}
