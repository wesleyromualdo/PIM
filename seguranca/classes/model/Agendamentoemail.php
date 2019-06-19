<?php
/**
 * Classe de mapeamento da entidade seguranca.agendamentoemail.
 *
 * @version $Id$
 * @since 2018.05.28
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Seguranca_Model_Agendamentoemail: sem descricao
 *
 * @package Seguranca\Model
 * @uses Simec\Db\Modelo
 * @author Felipe Tarchiani Ceravolo Chiavicatti <felipe.chiavicatti@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Seguranca_Model_Agendamentoemail($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Seguranca_Model_Agendamentoemail($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $aemstatus  - default: 'A'::bpchar
 * @property \Datetime(Y-m-d H:i:s) $aemcadastro  - default: now()
 * @property string $aemconteudo 
 * @property string $aemassunto 
 * @property string $aemsql 
 * @property string $aemdiasemana 
 * @property string $aemdiames 
 * @property string $aemperiodicidade 
 * @property string $aemtitulo 
 * @property string $usucpf 
 * @property int $sisid 
 * @property int $aemid  - default: nextval('agendamentoemail_aemid_seq'::regclass)
 */
class Seguranca_Model_Agendamentoemail extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'seguranca.agendamentoemail';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'aemid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
        'sisid' => array('tabela' => 'sistema', 'pk' => 'sisid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'aemstatus' => null,
        'aemcadastro' => null,
        'aemconteudo' => null,
        'aemassunto' => null,
        'aemsql' => null,
 //       'aemdiasemana' => null,
 //       'aemdiames' => null,
 //       'aemperiodicidade' => null,
        'aemtitulo' => null,
        'usucpf' => null,
        'sisid' => null,
        'aemid' => null,
    );

//     /**
//      * Validators.
//      *
//      * @param mixed[] $dados
//      * @return mixed[]
//      */
//     public function getCamposValidacao($dados = array())
//     {
//         return array(
//             'aemstatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
//             'aemcadastro' => array(),
//             'aemconteudo' => array(),
//             'aemassunto' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
//             'aemsql' => array(),
// //             'aemdiasemana' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
// //             'aemdiames' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
// //             'aemperiodicidade' => array(new Zend_Validate_StringLength(array('max' => 1))),
//             'aemtitulo' => array(new Zend_Validate_StringLength(array('max' => 255))),
//             'usucpf' => array(new Zend_Validate_StringLength(array('max' => 11))),
//             'sisid' => array('Digits'),
//             'aemid' => array('Digits'),
//         );
//     }

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
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }
    
    public function montaSQLLista($arPost){
    	$arPost['sisid'] = ($arPost['sisid'] ? $arPost['sisid'] : $_SESSION['sisid']);
    	
    	$where = array();
    	if ($arPost['sisid']) 				$where[] = " ae.sisid = {$arPost['sisid']}";
    	if ($arPost['aemtitulo']) 			$where[] = " ae.aemtitulo ilike '%{$arPost['aemtitulo']}%'";
    	if ($arPost['aemassunto']) 			$where[] = " ae.aemassunto ilike '%{$arPost['aemassunto']}%'";
    	if ($arPost['agsperiodicidade']) 	$where[] = " ac.agsperiodicidade IN ('". implode("', '", $arPost['agsperiodicidade']) ."')";
    	if ($arPost['aemstatus']) 			$where[] = " ae.aemstatus = '{$arPost['aemstatus']}'";
    	
    	$sql = "select 
    				ae.aemid,
    				ae.aemstatus,
					ae.aemtitulo,
					ae.aemassunto,
					CASE
    					WHEN ac.agsperiodicidade = 'mensal' THEN 'Mensal'
    					WHEN ac.agsperiodicidade = 'diario' THEN 'Diário'
    					WHEN ac.agsperiodicidade = 'semanal' THEN 'Semanal'
    				END	as agsperiodicidade,
					ac.agsperdetalhes,
					u.usunome
				from
					seguranca.agendamentoemail ae
				join seguranca.agendamentoscripts ac on ac.aemid = ae.aemid
				join seguranca.usuario u on u.usucpf = ae.usucpf
    			where
    				". implode(" AND ", $where) ."
    			order by
    				ae.aemid desc";
    	
    	return $sql;
    }

    public function pegarDadosEdicao($aemid){
    	$aemid = ($aemid ? $aemid : 0);
    	
    	$sql = "select
			    	ae.aemid,
			    	ae.aemtitulo,
			    	ae.aemsql,
			    	ae.aemassunto,
			    	ae.aemconteudo,
			    	ag.agsperiodicidade,
			    	ag.agsperdetalhes
		    	from
			    	seguranca.agendamentoemail ae
			    	join seguranca.agendamentoscripts ag on ag.aemid = ae.aemid
		    	where
		    		ae.aemid = {$aemid}";
    	$dados = $this->pegaLinha($sql);
    	
    	return ($dados ? $dados : array());
    }
    
}
