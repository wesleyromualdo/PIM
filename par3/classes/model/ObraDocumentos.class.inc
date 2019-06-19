<?php
/**
 * Classe de mapeamento da entidade par3.obra_documentos.
 *
 * @version $Id$
 * @since 2017.07.10
 */

/**
 * Par3_Model_Obra_documentos: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Renner Nascentes Tanizaki <rennertanizaki@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_ObraDocumentos($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Obra_documentos($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $odoid  - default: nextval('par3.obra_documentos_odoid_seq'::regclass)
 * @property int $obrid
 * @property int $arqid
 * @property int $otdid
 * @property string $odostatus  - default: 'A'::bpchar
 */
class Par3_Model_ObraDocumentos extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_documentos';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'odoid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'otdid' => array('tabela' => 'par3.obra_tipo_documento', 'pk' => 'otdid'),
        'arqid' => array('tabela' => 'arquivo', 'pk' => 'arqid'),
        'obrid' => array('tabela' => 'par3.obra', 'pk' => 'obrid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'odoid' => null,
        'obrid' => null,
        'arqid' => null,
        'otdid' => null,
        'odostatus' => null,
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
            'odoid' => array('Digits'),
            'obrid' => array('Digits'),
            'arqid' => array('Digits'),
            'otdid' => array('Digits'),
            'odostatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
        );
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
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function montarSQLSimples($arrPost)
    {
        $sql = "SELECT
                	oct.octordem as docordem,
                	otd.otddsc,
                	array_to_json(array_agg(arq.arqnome||'.'||arq.arqextensao)) arquivo,
                	array_to_json(array_agg(arq.arqid)) as arqid,
                	array_to_json(array_agg(arqdata || ' ' || arqhora)) as arqdata,
                	otd.otdid,
                	otd.otdid as otdidajuda,
                	otd.arqid as arqidmodelo,
                	obr.otpid as obratipo,
                	obr.obrid,
                	array_to_json(array_agg(odo.odoid)) as odoid
                FROM par3.obra obr
                INNER JOIN par3.obra_tipo 					otp ON otp.otpid = obr.otpid
                INNER JOIN par3.obra_configuracao_aba 		oca ON oca.otpid = otp.otpid AND oca.ocastatus = 'A'
                INNER JOIN par3.obra_confi_aba_x_tipo_doc 	oct ON oct.ocaid = oca.ocaid AND oct.octstatus = 'A'
                INNER JOIN par3.obra_tipo_documento 		otd ON otd.otdid = oct.otdid
                LEFT  JOIN par3.obra_documentos 			odo ON odo.otdid = otd.otdid AND odo.obrid = obr.obrid AND odo.odostatus = 'A'
                LEFT  JOIN public.arquivo 					arq ON arq.arqid = odo.arqid
                WHERE obr.obrid = {$arrPost['obrid']} AND otd.otdsituacao = 'A' AND otd.otdstatus = 'A'
                GROUP BY oct.octordem, otd.otddsc, otd.otdid, otd.otdajuda, otd.arqid, obr.obrid
                ORDER BY oct.octordem;";
// ver($sql);
        return $sql;
    }
    
    public function montarSQLSimplesNotaFNDE($arrPost)
    {
        $sql = "(
					select
						array_to_json(array_agg(a.arqnome||'.'||a.arqextensao)) arquivo,
				        array_to_json(array_agg(a.arqid)) as arqid,
				        --array_to_json(array_agg(a.arqdata || ' ' || a.arqhora)) as arqdata,
        		
				  		array_to_json(array_agg(' '::character)) as arquivo_nota,
				        array_to_json(array_agg(' '::character)) as arqid_nota,
				        --array_to_json(array_agg(' '::character)) as arqdata_nota,
        		
						'' as aadpparecer,
						'' as adpsituacao,
--						'' as aconteceu,        		
				        '' as usunome,
        				'' as adpdata,        
						od.otdid,
						null::smallint as adpid
						
					from
						par3.obra_documentos od
					join public.arquivo a on a.arqid = od.arqid AND a.arqstatus = 'A'
					where
						od.odostatus = 'A' and
						od.obrid = {$arrPost['obrid']} and
						od.otdid in (". OBRA_TIPO_DOCUMENTO_PROPRIEDADE_TERRENO .", ". OBRA_TIPO_DOCUMENTO_PROPRIEDADE_IMOVEL .") and
						od.arqid not in (select arqid from par3.obraanalisedocumentopropriedadeanexo)
					group by
						od.otdid, adpid, usunome
				)union all(
					select
						array_to_json(array_agg(a.arqnome||'.'||a.arqextensao)) arquivo,
				        array_to_json(array_agg(a.arqid)) as arqid,
				        --array_to_json(array_agg(a.arqdata || ' ' || a.arqhora)) as arqdata,
        		
				        array_to_json(array_agg(distinct a2.arqnome||'.'||a2.arqextensao)) arquivo_nota,       
				        array_to_json(array_agg(distinct a2.arqid)) as arqid_nota,
				        --array_to_json(array_agg(a2.arqdata || ' ' || a2.arqhora)) as arqdata_nota,
        		
						oap.adpparecer,
						case 
							when oap.adpsituacao = 'A' then 'APROVADO'
							when oap.adpsituacao = 'R' then 'REPROVADO'
							else 'SALVO'
						end as adpsituacao,
--        				'' as aconteceu,
				        u.usunome,
        				to_char(adpdata, 'DD/MM/YYYY') AS adpdata,
						null::smallint as odtid,
						oap.adpid
					from
						par3.obraanalisedocumentopropriedade oap
					join public.arquivo a2 on a2.arqid = oap.arqid_nota AND 
											  a2.arqstatus = 'A'
					join par3.obraanalisedocumentopropriedadeanexo oaa on oaa.adpid = oap.adpid
					join public.arquivo a on a.arqid = oaa.arqid AND 
											 a.arqstatus = 'A'
					join seguranca.usuario u on u.usucpf = oap.usucpf
					where
						oap.obrid = {$arrPost['obrid']}
					group by
						odtid, oap.adpid, u.usunome	
				)";
        
        return $sql;
    }

    public function montarSQLDadosTipoDocumento($arrPost)
    {
        $sql = "SELECT otdtamanho_minimo, otdtamanho_maximo, otdqtd_minima, otdqtd_maximo, otdajuda,
                (select count(otdid) FROM par3.obra_documentos od WHERE otd.otdid = od.otdid AND od.obrid = {$arrPost['obrid']} AND odostatus = 'A') count, array_to_json(array_agg(ex.extdesc)) extdesc
                FROM par3.obra_tipo_documento otd
                LEFT JOIN par3.obra_tipo_documento_x_extensao xex ON (xex.otdid = otd.otdid)
                LEFT JOIN public.extensao_arquivo ex ON (xex.extid = ex.extid)
                WHERE otd.otdid =  {$arrPost['otdid']} AND otdsituacao = 'A' AND otdstatus = 'A'
                GROUP BY otdtamanho_minimo, otdtamanho_maximo, otdqtd_minima, otdqtd_maximo, otd.otdid";
        return $sql;
    }

    public function sqlDocumentoAnalise( $obrid ){
        $sql = "SELECT
                	arq.arqid,
                	obr.obrid,
                	otd.otddsc,
                	arq.arqnome||'.'||arq.arqextensao as arquivo,
                	to_char(arq.arqdata, 'DD/MM/YYYY') || ' ' || arqhora as arqdata
                FROM par3.obra obr
                    inner join par3.obra_tipo 					otp ON otp.otpid = obr.otpid
                    inner join par3.obra_configuracao_aba 		oca ON oca.otpid = otp.otpid AND oca.ocastatus = 'A'
                    inner join par3.obra_confi_aba_x_tipo_doc 	oct ON oct.ocaid = oca.ocaid AND oct.octstatus = 'A'
                    inner join par3.obra_tipo_documento 		otd ON otd.otdid = oct.otdid and otd.otdid in (15, 20)
                    inner join par3.obra_documentos 			odo ON odo.otdid = otd.otdid AND odo.obrid = obr.obrid AND odo.odostatus = 'A'
                    inner join public.arquivo 					arq ON arq.arqid = odo.arqid
                WHERE obr.obrid = {$obrid} AND otd.otdsituacao = 'A' AND otd.otdstatus = 'A'
                ORDER BY oct.octordem";
        return $sql;
    }
    
    public function sqlDocumentoAnaliseSalvo( $obrid ){
        $sql = "select distinct oa.adpid, oa.adpparecer, oa.arqid_nota, a.arqnome||'.'||a.arqextensao as arquivo
                from par3.obraanalisedocumentopropriedade oa
                    inner join public.arquivo a on a.arqid = oa.arqid_nota
                where oa.obrid = $obrid and oa.adpsituacao is null order by oa.adpid desc";
        return $sql;
    }
}
